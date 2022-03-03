#version 330 core

layout (location = 0) out vec4 fragColor;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;
uniform vec2 u_cube;

const float FOV = 1.0;
const int MAX_STEPS = 300;
const float MAX_DIST = 900;
const float EPSILON = 0.001;

float fSphere(vec3 p, float r) {
	return length(p) - r;
}

float fTorus(vec3 p, float smallRadius, float largeRadius) {
	return length(vec2(length(p.xz) - largeRadius, p.y)) - smallRadius;
}

float vmax(vec3 v) {
	return max(max(v.x, v.y), v.z);
}
// Box: correct distance to corners
float fBox(vec3 p, vec3 b) 
{
	vec3 d = abs(p) - b;
	return length(max(d, vec3(0))) + vmax(min(d, vec3(0)));
}

// Cheap Box: distance to corners is overestimated
float fBox2(vec3 p, vec3 b) { //cheap box
	return vmax(abs(p) - b);
}

// Plane with normal n (n is normalized) at some distance from the origin
float fPlane(vec3 p, vec3 n, float distanceFromOrigin) {
	return dot(p, n) + distanceFromOrigin;
}

vec2 fOpUnionSoft(vec2 a, vec2 b, float r) 
{
	float e = max(r - abs(a.x - b.x), 0);
	vec2 res = vec2(min(a.x, b.x) - e*e*0.25/r, (a.x < b.x) ? a.y : b.y);
	return res;
}

vec2 fOpUnionMax(vec2 res1, vec2 res2) {
    return (res1.x > res2.x) ? res1 : res2;
}

vec2 fOpUnionMin(vec2 res1, vec2 res2) {
    return (res1.x < res2.x) ? res1 : res2;
}

void pR(inout vec2 p, float a) {
	p = cos(a)*p + sin(a)*vec2(p.y, -p.x);
}

float Displace(vec3 p)
{
	pR(p.yz, sin(u_time));
	return (sin(p.x * 0.81 + u_time) * sin(p.y * 0.8 + u_time) * sin(p.z * 0.8 + u_time));
}

vec2 map(vec3 p) 
{
    // plane
    float planeDist = fPlane(p, vec3(0, 1, 0), 10.0);
    float planeID = 4.0;
    vec2 plane = vec2(planeDist, planeID);
	
	//sphere
    vec3 ps = p + 0.2;
    ps.y -= 8;
	float displacement = Displace(p); // (radius + disp)
    float sphereDist = fSphere(ps, 13);
    float sphereID = 0.0;
    vec2 sphere = vec2(sphereDist, sphereID);
	
	/*
	//tor
	ps.z -= 40;
	float torDist = fTorus(ps, 5, 10);
	float torID = 1.0;
	vec2 tor = vec2(torDist, torID);
	*/
	
	//box
	ps.x += u_cube.x;
	ps.z += u_cube.y;
	float boxDist = fBox(ps, vec3(10, 10, 10));
	float boxID = 0.0;
	vec2 box = vec2(boxDist, boxID);
	
	//res
	vec2 res;
	//res = fOpUnionMax(box, sphere);
	res = fOpUnionSoft(box, sphere, 10.0);
	//res = fOpUnionSoft(res, tor, 10.0);
	res = fOpUnionMin(res, plane);
    return res;
}

vec2 rayMarch(vec3 ro, vec3 rd) 
{
    vec2 hit, object;
    for (int i = 0; i < MAX_STEPS; i++) {
        vec3 p = ro + object.x * rd;
        hit = map(p);
        object.x += hit.x;
        object.y = hit.y;
        if (abs(hit.x) < EPSILON || object.x > MAX_DIST) break;
    }
    return object;
}

vec3 getMaterial(vec3 p, float id) 
{
    vec3 m;
    switch (int(id)) {
		case 0:
		m = vec3(0.4, 0.4, 0.4); break; // gray
        case 1:
        m = vec3(0.0, 0.4, 0.0); break; // green
        case 2:
        m = vec3(0.9, 0.0, 0.0); break; //red
		case 3:
		m = vec3(0.17, 0.16, 0.66); break; // perpul
		case 4:
		m = vec3(0.2 + 0.4 * mod(floor(p.x / 10) + floor(p.z/ 10), 2.0)); break;
    }
    return m;
}

vec3 getNormal(vec3 p) 
{
    vec2 e = vec2(EPSILON, 0.0);
    vec3 n = vec3(map(p).x) - vec3(map(p - e.xyy).x, map(p - e.yxy).x, map(p - e.yyx).x);
    return normalize(n);
}

vec3 getLight(vec3 p, vec3 rd, vec3 color) 
{
    vec3 lightPos = vec3(10.0, 55.0, -20.0);
    vec3 L = normalize(lightPos - p);
    vec3 N = getNormal(p);
    vec3 V = -rd;
    vec3 R = reflect(-L, N);

    vec3 specColor = vec3(0.4);
    vec3 specular = specColor * pow(clamp(dot(R, V), 0.0, 1.0), 10.0);
    vec3 diffuse = color * clamp(dot(L, N), 0.0, 1.0);
    vec3 ambient = color * 0.05;
    vec3 fresnel = 0.25 * color * pow(1.0 + dot(rd, N), 3.0);

    // shadows
    //float d = rayMarch(p + N * 0.02, normalize(lightPos)).x;
    //if (d < length(lightPos - p)) return ambient + fresnel;

    return diffuse + ambient + specular + fresnel;
}


mat3 getCam(vec3 ro, vec3 lookAt) {
    vec3 camF = normalize(vec3(lookAt - ro));
    vec3 camR = normalize(cross(vec3(0, -1, 0), camF));
    vec3 camU = cross(camF, camR);
    return mat3(camR, camU, camF);
}


void mouseControl(inout vec3 ro) 
{
    vec2 m = u_mouse / u_resolution;
    pR(ro.xy, m.y * 1.5);
    pR(ro.xz, m.x * 1.5);
}

void render(inout vec3 col, in vec2 uv) 
{
    vec3 ro = vec3(36.0, 19.0, -36.0);
    mouseControl(ro);

    vec3 lookAt = vec3(0, 1, 0);
    vec3 rd = getCam(ro, lookAt) * normalize(vec3(uv, FOV));
    vec2 object = rayMarch(ro, rd);

    vec3 background = vec3(0.5, 0.8, 0.9);
    if (object.x < MAX_DIST) 
	{
        vec3 p = ro + object.x * rd;
        vec3 material = getMaterial(p, object.y);
        col += getLight(p, rd, material);
		//col += getLight2(p, material);
        // fog
        col = mix(col, background, 1.0 - exp(-0.00002 * object.x * object.x));
    } 
	else 
	{
        col += background - max(0.9 * rd.y, 0.01);
    }
}

void main() 
{
	vec2 uv = (2.0 * gl_FragCoord.xy - u_resolution.xy) / u_resolution.y;
    vec3 col;
    render(col, uv);

    // gamma correction
    col = pow(col, vec3(0.4545));
	
    fragColor = vec4(col, 1.0);
}