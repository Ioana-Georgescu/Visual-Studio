#version 330

layout(location = 0) out vec4 out_color;

uniform int object_code;
uniform sampler2D texture;
uniform vec3 camera_position;

uniform vec3 light_direction[50];
uniform vec3 light_position[50];
uniform vec3 light_color[50];
uniform vec3 eye_position;
uniform int lights_nr;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform int spotlight[50];
uniform float cut_off_angle[50];

in vec3 color;
in vec2 texcoord;
in vec3 world_position;
in vec3 world_normal;

void main()
{
	if ((object_code == 0) || (object_code == 10)) // monitor or light
	{
		out_color = vec4(color, 1);
	}
	else
	{
		vec3 N = normalize(world_normal);
		vec3 V = normalize(eye_position - world_position);
		vec3 new_color = vec3(1);

		float ambient_light = material_kd * 0.25;
		vec3 total_light = vec3(0);
		vec3 total_light_color = vec3(1);

		for (int i = 0; i < lights_nr; i++)
		{
			vec3 L = normalize(light_position[i] - world_position);
			vec3 H = normalize(L + V);

			float diffuse_light;
			float specular_light;
			float light_aten_fact;

			if (spotlight[i] == 0)
			{
				diffuse_light = material_kd * max(dot(N, L), 0);

				specular_light = 0.0;

				if (diffuse_light > 0)
				{
					specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess);
				}

				float light_distance = distance(world_position, light_position[i]);
				light_aten_fact = 1 / (light_distance * light_distance);
			}
			else
			{
				float cut_off = radians(cut_off_angle[i]);
				float spot_light = dot(-L, light_direction[i]);
				float spot_light_limit = cos(cut_off);

				if (spot_light > cos(cut_off))
				{
					diffuse_light = material_kd * max(dot(N, L), 0);

					specular_light = 0.0;

					if (diffuse_light > 0)
					{
						specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess);
					}

					float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
					light_aten_fact = pow(linear_att, 2);
				}
			}
			total_light += light_aten_fact * light_color[i] * (diffuse_light + specular_light);
		}
		new_color = 4 * total_light_color * (ambient_light + total_light);

		if (object_code != 12) // leaf uses color instead of texture
		{
			out_color = texture2D(texture, texcoord) * vec4(new_color, 1);
		}
		else
		{
			out_color = vec4(color * new_color, 1);
		}
	}
}