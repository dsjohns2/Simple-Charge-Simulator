#include <stdio.h>
#include <math.h>

typedef struct cell{
	float x;
	float y;
	float z;
	float Ex;
	float Ey;
	float Ez;
} cell;

typedef struct particle{
	float x;
	float y;
	float z;
	float vx;
	float vy;
	float vz;
} particle;

int main(int argc, char* argv[]){
	// Create electric field grid
	int max_coordinate = 5;
	int cells_per_unit = 5;
	int grid_size = max_coordinate * 2 * cells_per_unit + 1;
	float field_constant = 1.;
	float charge = 1.;
	cell grid[grid_size][grid_size][grid_size];
	int i, j, k;
	for(i=0; i<grid_size; i++){
		for(j=0; j<grid_size; j++){
			for(k=0; k<grid_size; k++){
				grid[i][j][k].x = -1*max_coordinate + i*(1./((float) cells_per_unit));
				grid[i][j][k].y = -1*max_coordinate + j*(1./((float) cells_per_unit));
				grid[i][j][k].z = -1*max_coordinate + k*(1./((float) cells_per_unit));

				float r_squared = pow(grid[i][j][k].x, 2) + pow(grid[i][j][k].y, 2) + pow(grid[i][j][k].z, 2);
				
				grid[i][j][k].Ex = field_constant * charge * grid[i][j][k].x / r_squared;
                grid[i][j][k].Ey = field_constant * charge * grid[i][j][k].y / r_squared;
                grid[i][j][k].Ez = field_constant * charge * grid[i][j][k].z / r_squared;
			}
		}
	}

	// Initialize particle
	particle p;
	p.x = 1;
	p.y = 0;
	p.z = 0.;
	p.vx = 0;
	p.vy = -.5;
	p.vz = 0.;

	// Move particle over time
	FILE* particle_fp = fopen("particle.dat", "w+");
	float num_timesteps = 2001;
	float timestep = 0.05;
	float test_charge = -.1;
	float test_mass = 1.;
	printf("Time : Position\n");
	for(i=0; i < num_timesteps; i++){
		float current_timestep = i*timestep;
		printf("%f : (%f, %f, %f)\n", current_timestep, p.x, p.y, p.z);
		fprintf(particle_fp, "%f %f %f %f %f %f %f\n", current_timestep, p.x, p.y, p.z, p.vx, p.vy, p.vz);

		int x_index = (p.x + max_coordinate) * cells_per_unit;
		int y_index = (p.y + max_coordinate) * cells_per_unit;
		int z_index = (p.z + max_coordinate) * cells_per_unit;

		if(x_index >= grid_size || z_index >= grid_size || y_index >= grid_size || x_index < 0 || z_index < 0 || y_index < 0){
			break;
		}
		
		float Fx = test_charge * grid[x_index][y_index][z_index].Ex; 
		float Fy = test_charge * grid[x_index][y_index][z_index].Ey; 
		float Fz = test_charge * grid[x_index][y_index][z_index].Ez; 

		float ax = Fx/test_mass;
		float ay = Fy/test_mass;
		float az = Fz/test_mass;

		p.x += p.vx * timestep + 0.5 * ax * pow(timestep, 2);
		p.y += p.vy * timestep + 0.5 * ay * pow(timestep, 2);
		p.z += p.vz * timestep + 0.5 * az * pow(timestep, 2);
		p.vx += ax * timestep;
		p.vy += ay * timestep;
		p.vz += az * timestep;
	}
	fclose(particle_fp);

	// Save field data
	FILE* field_fp = fopen("field.dat", "w+");
	for(i=0; i<grid_size; i++){
		j = k = (1 + max_coordinate) * cells_per_unit;
    	fprintf(field_fp, "%f %f %f %f %f %f\n", grid[i][j][k].x, grid[i][j][k].y, grid[i][j][k].z, grid[i][j][k].Ex, grid[i][j][k].Ey, grid[i][j][k].Ez);
    }
    fclose(field_fp);

	//Return
	return 0;
}
