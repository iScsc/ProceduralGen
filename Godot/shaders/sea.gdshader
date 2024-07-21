shader_type spatial;
render_mode specular_toon;

uniform vec2 map_dimensions = vec2(2.);
uniform vec2 map_dist = vec2(1.);

uniform vec3 base_color : source_color = vec3(0.01, 0.03, 0.05); //vec3(0, 0, 0.501961); // Navy Blue
uniform float alpha = 0.7;


float custom_cos(float x){
	//if (x <= 1e-2){
		//return 1.0;
	//}
	//
	//return 0.0;
	
	float cosx = cos(x);
	if (cosx >= 0.0){
		return cosx;
	}
	else {
		return 0.0;
	}
}


void vertex() {
	// Called for every vertex the material is visible on.
	
	float time_pulsation = 1.0;
	float time_angle = 2.0 * PI * time_pulsation * TIME + PI/2.0;
	
	
	
	vec2 waves_emitter_position = vec2(0.2, 0.2) * map_dimensions * map_dist;
	float r = length(VERTEX.xz - waves_emitter_position);
	
	float spatial_pulsation = 0.10;
	float spatial_angle = 2.0 * PI * spatial_pulsation * r;
	
	float amplitude = 1.0;
	
	VERTEX.y += amplitude * sin(time_angle - spatial_angle);//amplitude * sin(time_angle) * custom_cos(spatial_angle);
	
	//float k = VERTEX.y;
	//NORMAL = normalize(vec3(k - height(pos + vec2(0.1, 0.0), TIME), 0.1, k - height(pos + vec2(0.0, 0.1), TIME)));1
}

void fragment() {
	// Called for every pixel the material is visible on.
	float fresnel = sqrt(1.0 - dot(NORMAL, VIEW));
	
	ALBEDO = base_color + (0.1 * fresnel);
	//ALPHA = alpha;
	METALLIC = 0.0;
	ROUGHNESS = 0.01 * (1.0 - fresnel);
	RIM = 0.2;
}

//void light() {
	// Called for every pixel for every light affecting the material.
	// Uncomment to replace the default light processing function with this one.
//}
