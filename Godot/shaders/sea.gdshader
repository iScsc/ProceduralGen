shader_type spatial;
render_mode specular_toon; //, diffuse_toon;

uniform vec2 map_dimensions = vec2(2.);
uniform vec2 map_dist = vec2(1.);

uniform vec3 base_color : source_color = vec3(0.01, 0.03, 0.05); //vec3(0, 0, 0.501961); // Navy Blue
uniform float alpha = 0.7;



uniform sampler2D noise;



float waves1(vec2 position2d, float time, vec2 center_position){
	float time_pulsation = 1.0;
	float time_angle = 2.0 * PI * time_pulsation * time + PI/2.0;
	
	
	vec2 waves_emitter_position = (center_position - vec2(0.5)) * map_dimensions * map_dist;
	float r = length(position2d.xy - waves_emitter_position);
	
	float spatial_pulsation1 = 0.10;
	float spatial_angle = 2.0 * PI * spatial_pulsation1 * r;
	
	float spatial_pulsation2 = 0.05;
	
	float amplitude = 0.5;
	
	return amplitude * exp(-spatial_pulsation2 * r) * sin(time_angle - spatial_angle);
}



float waves2(vec2 position2d){
	position2d += texture(noise, position2d / 10.0).x * 2.0 - 1.0;
	vec2 wv = 1.0 - abs(sin(position2d));
	return pow(1.0 - pow(wv.x * wv.y, 0.65), 4.0);
}



float altitude(vec2 position2d, float time){
	float d = 0.0;
	
	
	//// FIRST WAVES TYPE : waves1(position2d, time)
	//d += waves1(position2d, time, vec2(0.7, -0.1));
	//d += waves1(position2d, time, vec2(1.1, 0.5));
	//d += waves1(position2d, time, vec2(0.5, 1.2));
	//d += waves1(position2d, time, vec2(-0.1, 0.2));
	
	
	//// SECOND WAVES TYPE : waves2(position2d)
	float amp1 = 0.3, amp2 = 0.3, amp3 = 0.2, amp4 = 0.2;
	float pul1 = 0.2, pul2 = 0.15, pul3 = 0.25, pul4 = 0.3;
	
	d += amp1 * waves2((position2d + time) * pul1);
	d += amp2 * waves2((position2d - time) * pul2);
	d += amp3 * waves2((position2d + time) * pul3);
	d += amp4 * waves2((position2d - time) * pul4);
	
	d /= (amp1 + amp2 + amp3 + amp4);
	
	float amplitude = 1.0;
	
	d *= amplitude;
	
	return d;
}



// Function to compute the actualized normal at given position, by averaging
// normals of the up_left plane and of the right_down one.
vec3 get_normal(vec3 position, float time){
	float cur_alt = position.y;
	
	vec2 left = vec2(-1.0, 0.0) * map_dist;
	vec2 up = vec2(0.0, -1.0) * map_dist;
	vec2 right = vec2(1.0, 0.0) * map_dist;
	vec2 down = vec2(0.0, 1.0) * map_dist;
	
	float left_alt = altitude(position.xz + left, time);
	float up_alt = altitude(position.xz + up, time);
	float right_alt = altitude(position.xz + right, time);
	float down_alt = altitude(position.xz + down, time);
	
	vec3 left_pos = vec3(position.x, 0, position.z) + vec3(left.x, left_alt, left.y);
	vec3 up_pos = vec3(position.x, 0, position.z) + vec3(up.x, up_alt, up.y);
	vec3 right_pos = vec3(position.x, 0, position.z) + vec3(right.x, right_alt, right.y);
	vec3 down_pos = vec3(position.x, 0, position.z) + vec3(down.x, down_alt, down.y);
	
	// Up left supposed triangle
	vec3 n1 = normalize(cross(up_pos - position, left_pos - position));
	
	// Down right supposed triangle
	vec3 n2 = normalize(cross(down_pos - position, right_pos - position));
	
	return (n1 + n2)/2.0; //(n1 + n2)/2.0; vec3(0.0, 1.0, 0.0);
}



void vertex() {
	// Called for every vertex the material is visible on.
	VERTEX.y += altitude(VERTEX.xz, TIME);
	
	NORMAL = get_normal(VERTEX, TIME);
}

void fragment() {
	// Called for every pixel the material is visible on.
	float fresnel = sqrt(1.0 - dot(NORMAL, VIEW));
	
	ALBEDO = base_color + (0.1 * fresnel);
	ALPHA = alpha;
	METALLIC = 0.0;
	ROUGHNESS = 0.01 * (1.0 - fresnel);
	RIM = 0.2;
}
