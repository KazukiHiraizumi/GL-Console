#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <cstring>
#include <vector>

#include "objview.h"

struct vec2{
	float x;
	float y;
	vec2(void){ x=y=0;}
	vec2& operator =(vec2& v){ x=v.x; y=v.y; return *this;}
};
struct vec3{
	float x;
	float y;
	float z;
	vec3(void){ x=y=z=0;}
	vec3& operator =(vec3& v){ x=v.x; y=v.y; z=v.z; return *this;}
	vec3& operator +=(vec3& v){ x+=v.x; y+=v.y; z+=v.z; return *this;}
	vec3& operator -=(vec3& v){ x-=v.x; y-=v.y; z-=v.z; return *this;}
	vec3& operator /=(float a){ x/=a; y/=a; z/=a; return *this;}
	float sqr(void){ return x*x+y*y+z*z;}
};

static int outsize=0;
static vec3 *out_vertices;
static vec2 *out_uvs;
static vec3 *out_normals;
static int vnumber=0;

int loadObj(char *path,float scale){
	std::vector<int> vertexIndices,uvIndices,normalIndices;
	std::vector<vec3> temp_vertices;
	std::vector<vec2> temp_uvs;
	std::vector<vec3> temp_normals;
	vec3 center;

	vnumber=0;
	FILE * file = fopen(path, "r");
	if( file == NULL){
		fprintf(stderr,"Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return vnumber;
	}
	char lineHeader[128];
ParseLine:
	int res=fscanf(file, "%s",lineHeader);
	if(res==EOF){
		fclose(file);
		goto CenterVert;
	}
	if(strcmp(lineHeader,"v")==0){
		vec3 vertex;
		fscanf(file, "%f %f %f\n",&vertex.x,&vertex.y,&vertex.z);
		temp_vertices.push_back(vertex);
		center+=vertex;
	}
	else if(strcmp(lineHeader,"vt")==0){
		vec2 uv;
		fscanf(file,"%f %f\n",&uv.x,&uv.y);
		uv.y=-uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
		temp_uvs.push_back(uv);
	}
	else if(strcmp(lineHeader,"vn")==0){
		vec3 normal;
		fscanf(file,"%f %f %f\n",&normal.x,&normal.y,&normal.z);
		temp_normals.push_back(normal);
	}
	else if(strcmp(lineHeader,"f")==0){
		char fset[3][20];
		int vertexIndex[3], uvIndex[3], normalIndex[3];
//		int matches=fscanf(file,"%d/%d/%d %d/%d/%d %d/%d/%d\n",&vertexIndex[0],&uvIndex[0],&normalIndex[0],&vertexIndex[1],&uvIndex[1],&normalIndex[1],&vertexIndex[2],&uvIndex[2],&normalIndex[2]);
		int matches=fscanf(file,"%s %s %s\n",fset[0],fset[1],fset[2]);
		if(matches!=3){
			printf("File can't be read by our simple parser :-( Try exporting with other options\n");
			fclose(file);
			return vnumber;
		}
		vertexIndex[0]=atoi(fset[0]);
		vertexIndex[1]=atoi(fset[1]);
		vertexIndex[2]=atoi(fset[2]);
		uvIndex[0]=atoi(strchr(fset[0],'/')+1);
		uvIndex[1]=atoi(strchr(fset[1],'/')+1);
		uvIndex[2]=atoi(strchr(fset[2],'/')+1);
		normalIndex[0]=atoi(strrchr(fset[0],'/')+1);
		normalIndex[1]=atoi(strrchr(fset[1],'/')+1);
		normalIndex[2]=atoi(strrchr(fset[2],'/')+1);
		vertexIndices.push_back(vertexIndex[0]);
		vertexIndices.push_back(vertexIndex[1]);
		vertexIndices.push_back(vertexIndex[2]);
		uvIndices.push_back(uvIndex[0]);
		uvIndices.push_back(uvIndex[1]);
		uvIndices.push_back(uvIndex[2]);
		normalIndices.push_back(normalIndex[0]);
		normalIndices.push_back(normalIndex[1]);
		normalIndices.push_back(normalIndex[2]);
	}
	else{ //skip rest of the line
		char stupidBuffer[1000];
		fgets(stupidBuffer, 1000, file);
	}
	goto ParseLine;
CenterVert:
	center/=temp_vertices.size();
	fprintf(stderr,"vertices=%d %f %f %f\n",temp_vertices.size(),center.x,center.y,center.z);
	float maxq=0;
	for(int i=0;i<temp_vertices.size();i++){
		temp_vertices[i]-=center;
		float q=temp_vertices[i].sqr();
		if(maxq<q) maxq=q;
	}
	scale=sqrt(maxq)/scale;
	for(int i=0;i<temp_vertices.size();i++){
		temp_vertices[i]/=scale;
	}
ArrangeOutput:
	int outs=vertexIndices.size();
	fprintf(stderr,"object vert numbers:%d\n",outs);
	if(outsize<outs){
		if(outsize>0){
			delete out_normals;
			delete out_uvs;
			delete out_vertices;
		}
		outsize=outs;
		out_vertices=new vec3[outs];
		out_uvs=new vec2[outs];
		out_normals=new vec3[outs];
	}
	for(int i=0;i<outs;i++){
		int vertexIndex=vertexIndices[i];
		int uvIndex=uvIndices[i];
		int normalIndex=normalIndices[i];
		out_vertices[i]=temp_vertices[vertexIndex-1];
		if(temp_uvs.size()>0) out_uvs[i]=temp_uvs[uvIndex-1];
		out_normals[i]=temp_normals[normalIndex-1];
	}
	return vnumber=vertexIndices.size();
}

void drawObj(float a,float rx,float ry,float rz){
	if(vnumber==0) return;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(a,rx,ry,rz);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, (float *)out_vertices);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, (float *)out_normals);
	glDrawArrays(GL_TRIANGLES,0,vnumber);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
