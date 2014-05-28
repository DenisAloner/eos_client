uniform sampler2D Map;

varying vec2 blurCoordinates[5];
 
void main()
{
	vec4 sum = vec4(0.0,0.0,0.0,1.0);
	sum += texture2D(Map, blurCoordinates[0]) * 0.204164;
	sum += texture2D(Map, blurCoordinates[1]) * 0.304005;
	sum += texture2D(Map, blurCoordinates[2]) * 0.304005;
	sum += texture2D(Map, blurCoordinates[3]) * 0.093913;
	sum += texture2D(Map, blurCoordinates[4]) * 0.093913;
	gl_FragColor = sum;
}