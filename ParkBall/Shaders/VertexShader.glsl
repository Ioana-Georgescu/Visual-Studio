#version 330

// spatiu ob
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float ElapsedTime;
uniform float speed;

uniform vec3 object_pos;
uniform vec3 object_size;
uniform vec3 object_color;
uniform int object_code;

out vec3 color;
out vec2 texcoord;
out vec3 world_position;
out vec3 world_normal;

float frizz1(vec3 v)
{
    return fract( sin( dot(v, vec3(56.9898, 43.1554, 91.2329329))) * 54564.067321);
}

float frizz2(vec3 v)
{
    return fract( (v[0] + v[1] + v[2])  * 754.4974275);
}

void main()
{
	vec3 finalColor = object_color;
	vec4 position;
	vec3 distortion;
	vec3 deformedCoords;

	if ((object_code == 1) || (object_code >= 10)) // normal, light, player roll or leaf
		position = Projection * View * Model * vec4(v_position, 1.0);

	else if (object_code == 0) // monitor
	{
		vec3 displayAt;
		for (int i = 0; i < 3; i++)
		{
			displayAt[i] = v_position[i] * object_size[i] + object_pos[i];
		}
		position = vec4(displayAt, 1.0);
	}

	else if (object_code == 2) // stuck on max speed
	{
		deformedCoords = vec3( v_position[0] * frizz1(v_normal + vec3(ElapsedTime)), v_position[1], v_position[2] + frizz1(v_normal + vec3(ElapsedTime)) );
		position = Projection * View * Model * vec4( deformedCoords, 1.0);
	}

	else if (object_code == 3) // plus fuel
	{
		distortion = v_normal * frizz2(v_normal + vec3(ElapsedTime));
		deformedCoords = v_position + vec3( distortion[0], 0, distortion[2] / 2 );
		position = Projection * View * Model * vec4(deformedCoords, 1.0);
	}

	else if (object_code == 4) // minus fuel
	{
		distortion = v_normal * frizz1(ElapsedTime * v_normal);
		position = Projection * View * Model * vec4(v_position - distortion, 1.0);
	}

	else if (object_code == 5) // plus life
	{
		distortion = v_normal * (frizz2(ElapsedTime * v_normal) + frizz1(v_normal + vec3(ElapsedTime)));
		deformedCoords = (v_position + distortion) / 2;
		position = Projection * View * Model * vec4(deformedCoords, 1.0);
	}

	else if (object_code == 6) // minus life
	{
		distortion = v_normal * (frizz2(v_normal + vec3(ElapsedTime)) - frizz1(ElapsedTime * v_normal));
		position = Projection * View * Model * vec4(v_position + distortion, 1.0);
	}

	else if (object_code == 7) // ghost
	{
		deformedCoords = vec3( v_position[0] * frizz2(v_normal + vec3(ElapsedTime)), v_position[1] + frizz2(v_normal + vec3(ElapsedTime)), v_position[2] );
		position = Projection * View * Model * vec4( deformedCoords, 1.0);
	}

	else if (object_code == 8) // orange + ghost
	{
		deformedCoords = vec3( v_position[0] * frizz1(v_normal + vec3(ElapsedTime)), v_position[1] + frizz2(v_normal + vec3(ElapsedTime)), v_position[2]);
		position = Projection * View * Model * vec4( deformedCoords, 1.0);
	}

	else if (object_code == 9) // has hit wall
	{
		float squish = sin(ElapsedTime + speed);
		deformedCoords = vec3( v_position[0] * squish, v_position[1] * squish, v_position[2] * squish);
		position = Projection * View * Model * vec4( deformedCoords, 1.0);
	}

	gl_Position = position;
	color = finalColor;
	texcoord = v_texture_coord;

	world_position = vec3(Model * vec4(v_position, 1));
	world_normal = normalize(vec3(Model * vec4(v_normal, 0)));
}
