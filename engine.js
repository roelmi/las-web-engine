'use strict';
/* LAS Web Engine / Island Trial: original demo, no game code or commercial assets. */
(()=>{
const $=id=>document.getElementById(id), canvas=$('gl'), error=$('error');
let gl;
try{gl=canvas.getContext('webgl2',{antialias:false,alpha:false,powerPreference:'high-performance'});if(!gl)throw Error('WebGL 2 is unavailable on this device/browser.');}catch(e){error.hidden=false;error.textContent=e.message;return;}
const vs=`#version 300 es
precision highp float;layout(location=0)in vec3 p;layout(location=1)in vec3 n;layout(location=2)in vec3 aColor;out vec3 vColor;uniform mat4 uMVP;uniform mat4 uModel;out vec3 vN;out vec3 vW;void main(){vec4 wp=uModel*vec4(p,1.);vW=wp.xyz;vN=mat3(uModel)*n;vColor=aColor;gl_Position=uMVP*vec4(p,1.);}`;
const fs=`#version 300 es
precision highp float;in vec3 vN;in vec3 vW;in vec3 vColor;uniform vec3 uColor;uniform float uTime;uniform float uGlow;out vec4 o;void main(){vec3 norm=normalize(vN);float lam=max(dot(norm,normalize(vec3(-.4,.85,.35))),0.);float light=.54+.46*lam;float fog=smoothstep(7.,27.,length(vW.xz));vec3 base=uColor*vColor;vec3 c=mix(base*light+base*uGlow*(.3+.15*sin(uTime*4.)),vec3(.43,.72,.70),fog*.23);o=vec4(pow(c,vec3(.94)),1.);}`;
function shader(type,src){const s=gl.createShader(type);gl.shaderSource(s,src);gl.compileShader(s);if(!gl.getShaderParameter(s,gl.COMPILE_STATUS))throw Error(gl.getShaderInfoLog(s));return s;}
let prog;try{prog=gl.createProgram();gl.attachShader(prog,shader(gl.VERTEX_SHADER,vs));gl.attachShader(prog,shader(gl.FRAGMENT_SHADER,fs));gl.linkProgram(prog);if(!gl.getProgramParameter(prog,gl.LINK_STATUS))throw Error(gl.getProgramInfoLog(prog));}catch(e){error.hidden=false;error.textContent='Graphics setup failed: '+e.message;return;}
gl.useProgram(prog);const loc={mvp:gl.getUniformLocation(prog,'uMVP'),model:gl.getUniformLocation(prog,'uModel'),color:gl.getUniformLocation(prog,'uColor'),time:gl.getUniformLocation(prog,'uTime'),glow:gl.getUniformLocation(prog,'uGlow')};
function mesh(verts,idx){let vao=gl.createVertexArray();gl.bindVertexArray(vao);let vb=gl.createBuffer();gl.bindBuffer(gl.ARRAY_BUFFER,vb);gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(verts),gl.STATIC_DRAW);let ib=gl.createBuffer();gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,ib);gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,new Uint16Array(idx),gl.STATIC_DRAW);for(let i=0;i<2;i++){gl.enableVertexAttribArray(i);gl.vertexAttribPointer(i,3,gl.FLOAT,false,24,i*12)}gl.disableVertexAttribArray(2);gl.vertexAttrib3f(2,1,1,1);gl.bindVertexArray(null);return {vao,count:idx.length};}
function box(){let v=[],ix=[];let faces=[[[0,0,1],[[-1,-1,1],[1,-1,1],[1,1,1],[-1,1,1]]],[[0,0,-1],[[1,-1,-1],[-1,-1,-1],[-1,1,-1],[1,1,-1]]],[[1,0,0],[[1,-1,1],[1,-1,-1],[1,1,-1],[1,1,1]]],[[-1,0,0],[[-1,-1,-1],[-1,-1,1],[-1,1,1],[-1,1,-1]]],[[0,1,0],[[-1,1,1],[1,1,1],[1,1,-1],[-1,1,-1]]],[[0,-1,0],[[-1,-1,-1],[1,-1,-1],[1,-1,1],[-1,-1,1]]]];faces.forEach(([n,pts])=>{let k=v.length/6;pts.forEach(p=>v.push(...p,...n));ix.push(k,k+1,k+2,k,k+2,k+3)});return mesh(v,ix)}
function cone(sides=8){let v=[0,1,0,0,1,0],ix=[];for(let i=0;i<sides;i++){let a=i/sides*Math.PI*2,x=Math.cos(a),z=Math.sin(a);v.push(x,-1,z,x*.8,.5,z*.8)}for(let i=0;i<sides;i++)ix.push(0,1+i,1+(i+1)%sides);return mesh(v,ix)}
const cube=box(),pine=cone(9);
const I=()=>[1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1];
function mul(a,b){let o=new Array(16).fill(0);for(let c=0;c<4;c++)for(let r=0;r<4;r++)for(let k=0;k<4;k++)o[c*4+r]+=a[k*4+r]*b[c*4+k];return o}
function perspective(fovy,aspect,near,far){let f=1/Math.tan(fovy/2),o=new Array(16).fill(0);o[0]=f/aspect;o[5]=f;o[10]=(far+near)/(near-far);o[11]=-1;o[14]=2*far*near/(near-far);return o}
function look(eye,center){let z=norm(sub(eye,center)),x=norm(cross([0,1,0],z)),y=cross(z,x);return [x[0],y[0],z[0],0,x[1],y[1],z[1],0,x[2],y[2],z[2],0,-dot(x,eye),-dot(y,eye),-dot(z,eye),1]}
const sub=(a,b)=>a.map((v,i)=>v-b[i]),dot=(a,b)=>a.reduce((s,v,i)=>s+v*b[i],0),cross=(a,b)=>[a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]],norm=a=>{let d=Math.hypot(...a)||1;return a.map(v=>v/d)};
function transform(x,y,z,sx,sy,sz,rot=0){let c=Math.cos(rot),s=Math.sin(rot);return [c*sx,0,-s*sx,0,0,sy,0,0,s*sz,0,c*sz,0,x,y,z,1]}
let vp=I(),now=0,draws=0;const modelBuffer=new Float32Array(16),mvpBuffer=new Float32Array(16);function draw(m,x,y,z,sx,sy,sz,color,rot=0,glow=0){let model=transform(x,y,z,sx,sy,sz,rot);modelBuffer.set(model);mvpBuffer.set(mul(vp,model));gl.uniformMatrix4fv(loc.model,false,modelBuffer);gl.uniformMatrix4fv(loc.mvp,false,mvpBuffer);gl.uniform3fv(loc.color,color);gl.uniform1f(loc.glow,glow);gl.bindVertexArray(m.vao);gl.drawElements(gl.TRIANGLES,m.count,gl.UNSIGNED_SHORT,0);draws++;}
const color={grass:[.36,.67,.43],sand:[.88,.78,.49],water:[.18,.49,.64],leaf:[.13,.47,.32],leaf2:[.25,.59,.39],wood:[.45,.30,.20],stone:[.52,.62,.62],crystal:[.99,.86,.31],hero:[.22,.52,.82],skin:[.97,.77,.55],enemy:[.76,.30,.44],gate:[.25,.91,.86]};
const rand=(x,z)=>{let n=Math.sin(x*127.1+z*311.7)*43758.5453;return n-Math.floor(n)};
const world=[],blocked=[];for(let x=-12;x<=12;x++)for(let z=-12;z<=12;z++){let r=Math.hypot(x,z),isWater=r>11.2||(r>9.5&&rand(x,z)>.78);if(r>12.3)continue;let tree=!isWater&&r>3.2&&rand(x+7,z+3)>.79;let rock=!isWater&&!tree&&rand(x-11,z+9)>.93;world.push({x,z,isWater,tree,rock,r});if(tree||rock||isWater)blocked.push({x,z})}
// One static colorized mesh replaces hundreds of per-tile WebGL draw calls.
function buildTerrain(){
 const vertices=[],indices=[];
 const faceData=[[[0,0,1],[[-1,-1,1],[1,-1,1],[1,1,1],[-1,1,1]]],[[0,0,-1],[[1,-1,-1],[-1,-1,-1],[-1,1,-1],[1,1,-1]]],[[1,0,0],[[1,-1,1],[1,-1,-1],[1,1,-1],[1,1,1]]],[[-1,0,0],[[-1,-1,-1],[-1,-1,1],[-1,1,1],[-1,1,-1]]],[[0,1,0],[[-1,1,1],[1,1,1],[1,1,-1],[-1,1,-1]]],[[0,-1,0],[[-1,-1,-1],[1,-1,-1],[1,-1,1],[-1,-1,1]]]];
 for(const q of world){const {x,z,isWater,r}=q, tint=isWater?color.water:r>9?color.sand:((x+z)%3===0?[.36,.65,.41]:color.grass);
   for(const [n,pts] of faceData){const base=vertices.length/9;for(const v of pts)vertices.push(x+v[0]*.505,-.39+v[1]*.36,z+v[2]*.505,...n,...tint);indices.push(base,base+1,base+2,base,base+2,base+3)}
 }
 const vao=gl.createVertexArray();gl.bindVertexArray(vao);
 const vbo=gl.createBuffer();gl.bindBuffer(gl.ARRAY_BUFFER,vbo);gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(vertices),gl.STATIC_DRAW);
 const ibo=gl.createBuffer();gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,ibo);gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,new Uint16Array(indices),gl.STATIC_DRAW);
 for(let i=0;i<3;i++){gl.enableVertexAttribArray(i);gl.vertexAttribPointer(i,3,gl.FLOAT,false,36,i*12)}gl.bindVertexArray(null);
 return {vao,count:indices.length};
}
const terrain=buildTerrain();
let hero={x:0,z:0,dir:0,hp:3,inv:0,attack:0,dash:0},crystals=[],enemies=[];
for(let i=0;i<8;i++){let a=i*2.399+0.4,r=3.6+i*.68;crystals.push({x:Math.cos(a)*r,z:Math.sin(a)*r,got:false})}
for(let i=0;i<7;i++){let a=i*2.399+1.7;enemies.push({x:Math.cos(a)*5.8,z:Math.sin(a)*5.8,alive:true,t:i*1.4})}
let keys={},joy={x:0,y:0},playing=false,won=false,last=0,frames=0,ft=0,count=0,quality=.72,autoQuality=true,slowWindows=0,fastWindows=0,viewW=0,viewH=0;
function notify(s){const m=$('message');m.textContent=s;m.style.opacity=1;clearTimeout(notify.timer);notify.timer=setTimeout(()=>m.style.opacity=0,2200)}
function updateHud(){$('hearts').textContent=Array.from({length:3},(_,i)=>i<hero.hp?'♥':'♡').join(' ');$('count').textContent=count}
function reset(){hero={x:0,z:0,dir:0,hp:3,inv:0,attack:0,dash:0};crystals.forEach(c=>c.got=false);enemies.forEach(e=>e.alive=true);count=0;won=false;updateHud()}
function attack(){if(!playing||hero.attack>0)return;hero.attack=.25;for(let e of enemies){let dx=e.x-hero.x,dz=e.z-hero.z;if(e.alive&&Math.hypot(dx,dz)<1.75){e.alive=false;notify('Creature defeated!')}}}
function dash(){if(playing&&hero.dash<=0)hero.dash=.28}
function movable(x,z){if(Math.hypot(x,z)>10)return false;for(let b of blocked)if(Math.abs(x-b.x)<.57&&Math.abs(z-b.z)<.57)return false;return true}
function update(dt){if(!playing)return;hero.inv=Math.max(0,hero.inv-dt);hero.attack=Math.max(0,hero.attack-dt);hero.dash=Math.max(0,hero.dash-dt);
let dx=(keys.ArrowRight||keys.KeyD?1:0)-(keys.ArrowLeft||keys.KeyA?1:0)+joy.x,dz=(keys.ArrowDown||keys.KeyS?1:0)-(keys.ArrowUp||keys.KeyW?1:0)+joy.y;
let gp=navigator.getGamepads?.()[0];if(gp){dx+=Math.abs(gp.axes[0])>.15?gp.axes[0]:0;dz+=Math.abs(gp.axes[1])>.15?gp.axes[1]:0;if(gp.buttons[0]?.pressed)attack();if(gp.buttons[1]?.pressed)dash()}
let len=Math.hypot(dx,dz);if(len>.05){dx/=len;dz/=len;hero.dir=Math.atan2(dx,dz);let speed=hero.dash>0?9:3.55,nx=hero.x+dx*dt*speed,nz=hero.z+dz*dt*speed;if(movable(nx,hero.z))hero.x=nx;if(movable(hero.x,nz))hero.z=nz}
for(let c of crystals)if(!c.got&&Math.hypot(c.x-hero.x,c.z-hero.z)<.8){c.got=true;count++;updateHud();notify(count===8?'All crystals! Go to the portal.':`Crystal ${count}/8 collected ✦`)}
for(let e of enemies){if(!e.alive)continue;e.t+=dt;let dd=Math.hypot(hero.x-e.x,hero.z-e.z);if(dd<5&&dd>1.1){let s=.75*dt,nx=e.x+(hero.x-e.x)/dd*s,nz=e.z+(hero.z-e.z)/dd*s;if(movable(nx,nz)){e.x=nx;e.z=nz}}if(dd<.84&&hero.inv<=0){hero.hp--;hero.inv=1.4;updateHud();notify('Ouch!');if(hero.hp<=0){playing=false;openMenu('TRY AGAIN','Your adventure ended. Try again!')}}}
if(count===8&&Math.hypot(hero.x,hero.z+2.2)<1.1){won=true;playing=false;openMenu('ISLAND COMPLETE','You collected every crystal. The WebGL2 engine runs locally on your device!')}}
function render(t){const ratio=Math.min(window.devicePixelRatio||1,1.75)*quality,w=Math.max(1,Math.floor(innerWidth*ratio)),h=Math.max(1,Math.floor(innerHeight*ratio));if(canvas.width!==w||canvas.height!==h){canvas.width=w;canvas.height=h}gl.viewport(0,0,w,h);gl.enable(gl.DEPTH_TEST);gl.enable(gl.CULL_FACE);gl.clearColor(.48,.75,.76,1);gl.clear(gl.COLOR_BUFFER_BIT|gl.DEPTH_BUFFER_BIT);gl.useProgram(prog);gl.uniform1f(loc.time,t);let eye=[hero.x+9.3,12.6,hero.z+12.4],center=[hero.x,0,hero.z];vp=mul(perspective(Math.PI/3.5,w/h,.1,80),look(eye,center));draws=0;
// Static terrain is uploaded once, then rendered in ONE indexed call per frame.
draw(terrain,0,0,0,1,1,1,[1,1,1]);
for(let q of world){let {x,z,isWater,tree,rock}=q;if(isWater)continue;
if(tree){draw(cube,x,.42,z,.15,.7,.15,color.wood);draw(pine,x,1.45,z,.66,.83,.66,(x+z)%2?color.leaf:color.leaf2);draw(pine,x,2.12,z,.53,.68,.53,color.leaf2)}else if(rock){draw(cube,x,.11,z,.44,.38,.37,color.stone,rand(x,z))}else if(rand(x+34,z+4)>.91){draw(cube,x,.025,z,.13,.06,.13,[.8,.79,.48])}}
// portal
let glowing=count===8;draw(cube,0,.1,-2.2,.8,.12,.8,[.39,.67,.66]);draw(cube,-.7,.9,-2.2,.15,.85,.2,color.stone);draw(cube,.7,.9,-2.2,.15,.85,.2,color.stone);draw(cube,0,1.75,-2.2,.88,.15,.2,color.stone);draw(cube,0,.9,-2.2,.54,.75,.05,glowing?color.gate:[.16,.40,.47],0,glowing?1.2:.1);
for(let c of crystals)if(!c.got){let bob=.19*Math.sin(t*2.7+c.x);draw(pine,c.x,.65+bob,c.z,.28,.36,.28,color.crystal,t*.8,1.4);draw(cube,c.x,.03,c.z,.34,.016,.34,[.68,.59,.27])}
for(let e of enemies)if(e.alive){let bob=.05*Math.sin(t*5+e.t);draw(cube,e.x,.43+bob,e.z,.49,.39,.45,color.enemy,Math.sin(e.t)*.2);draw(cube,e.x-.17,.65+bob,e.z+.42,.07,.08,.05,[.99,.96,.84]);draw(cube,e.x+.17,.65+bob,e.z+.42,.07,.08,.05,[.99,.96,.84]);}
let flash=hero.inv>0&&Math.floor(t*12)%2===0; if(!flash){draw(cube,hero.x,.58,hero.z,.32,.46,.31,color.hero,hero.dir);draw(cube,hero.x,1.18,hero.z,.29,.26,.29,color.skin,hero.dir);draw(cube,hero.x,1.42,hero.z-.07,.33,.15,.35,[.30,.25,.14],hero.dir);if(hero.attack>0){let xx=hero.x+Math.sin(hero.dir)*.9,zz=hero.z+Math.cos(hero.dir)*.9;draw(cube,xx,.8,zz,.08,.65,.11,[.89,.98,1],-hero.dir+.6,1.4)}}
}
function loop(ts){const t=ts*.001,dt=Math.min(.045,t-last||.016);last=t;update(dt);render(t);frames++;ft+=dt;if(ft>=1.35){const fps=frames/ft;if(autoQuality&&playing){if(fps<25){slowWindows++;fastWindows=0}else if(fps>29.3){fastWindows++;slowWindows=0}else{slowWindows=fastWindows=0}if(slowWindows>=2){quality=Math.max(.52,Math.round((quality-.08)*100)/100);slowWindows=0}if(fastWindows>=5){quality=Math.min(.82,Math.round((quality+.04)*100)/100);fastWindows=0}}$('performance').textContent=Math.round(fps)+' FPS · '+(autoQuality?'AUTO ':'')+Math.round(quality*100)+'% · WebGL2';frames=0;ft=0}requestAnimationFrame(loop)}
function openMenu(title,description){$('overlay').classList.remove('hidden');$('overlay').querySelector('h1').innerHTML=title;$('overlay').querySelector('p').textContent=description;$('play').textContent='PLAY AGAIN'}
$('play').addEventListener('click',()=>{autoQuality=$('quality').value==='auto';quality=autoQuality?.72:Number($('quality').value);slowWindows=fastWindows=0;reset();playing=true;$('overlay').classList.add('hidden');notify('Collect 8 crystals · Defeat enemies · Open the gate');});$('pause').addEventListener('click',()=>{if(!playing)return;playing=false;openMenu('PAUSED','Your progress is paused. Select Play Again to restart the challenge.')});
addEventListener('keydown',e=>{keys[e.code]=true;if(['Space','ArrowUp','ArrowDown','ArrowLeft','ArrowRight'].includes(e.code))e.preventDefault();if(e.code==='Space')attack();if(e.code==='ShiftLeft'||e.code==='ShiftRight')dash()});addEventListener('keyup',e=>keys[e.code]=false);
const joystick=$('joystick'),stick=$('stick');function moveStick(e){const r=joystick.getBoundingClientRect(),x=e.clientX-r.left-r.width/2,y=e.clientY-r.top-r.height/2,scale=Math.min(1,49/(Math.hypot(x,y)||1));joy.x=x*scale/49;joy.y=y*scale/49;stick.style.transform=`translate(${joy.x*49}px,${joy.y*49}px)`}joystick.addEventListener('pointerdown',e=>{joystick.setPointerCapture(e.pointerId);moveStick(e)});joystick.addEventListener('pointermove',e=>{if(joystick.hasPointerCapture(e.pointerId))moveStick(e)});function release(){joy.x=joy.y=0;stick.style.transform='translate(0,0)'}joystick.addEventListener('pointerup',release);joystick.addEventListener('pointercancel',release);
$('btnA').addEventListener('pointerdown',e=>{e.preventDefault();attack()});$('btnB').addEventListener('pointerdown',e=>{e.preventDefault();dash()});canvas.addEventListener('webglcontextlost',e=>{e.preventDefault();playing=false;error.hidden=false;error.textContent='Graphics context lost. Reload the page to restart.'});
document.addEventListener('visibilitychange',()=>{if(document.hidden){joy.x=joy.y=0;keys={};stick.style.transform='translate(0,0)'}});
requestAnimationFrame(loop);
})();
