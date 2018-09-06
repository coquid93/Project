#include "gl/glew.h"
#include "Geometry.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <gl/freeglut.h>
#include <math.h>

#pragma warning(disable:4996)

using namespace std;
using namespace respace;

Geometry::Geometry()
{
	Vertices.clear();
	Normals.clear();
	indices.clear();
}
Geometry::~Geometry()
{
	Vertices.clear();
	Normals.clear();
	TextureCoord.clear();
	indices.clear();
	type.clear();
	triangles.clear();
}
void Geometry::loadObj(std::string filename)
{
	fstream file;
	string line;
	file.open(filename.c_str(),fstream::in);
	if(!file.is_open())
	{
		string str="Model File Not Found: ";
		str+=filename;
		return;
	}
	unsigned int n=0;
	Vec3f v;
	Vec2f vt;
	Vec3f vn;

	//face
	Vec3i iv;
	Vec3i ivt;
	Vec3i ivn;
	Vec3i quad_temp;
	std::vector<int> temp_indices;
	temp_indices.clear();
	temp_indices.clear();
	char buf[128];
	while(!file.eof())
	{
		getline(file,line);
		if(line.length()>2)
		switch(line.at(0))
		{
			case 'm':
				  sscanf(line.c_str(), "%s %s", buf, buf);
			case 'v':
					//vertex or tex coord
					if(line.at(1)==' ')
					{
						//vertex
						n=sscanf(line.c_str(),"v %f %f %f",&v.x,&v.y,&v.z);
						if(n!=3)
						{
							v.x=0;
							v.y=0;
							v.z=0;
							printf("failv!\n");
						}
						Vertices.push_back(v);
						vert_Buf.push_back(v.x);
						vert_Buf.push_back(v.y);
						vert_Buf.push_back(v.z);
					}
					else if(line.at(1)=='n')
					{
						//normal
						n=sscanf(line.c_str(),"vn %f %f %f",&vn.x,&vn.y,&vn.z);
						if(n!=3)
						{
							vn.x=0;
							vn.y=0;
							vn.z=0;
							printf("normal failn!\n");
						}
						Normals.push_back(vn);
						norm_Buf.push_back(vn.x);
						norm_Buf.push_back(vn.y);
						norm_Buf.push_back(vn.z);
					}
					else if(line.at(1)=='t')
					{
						//texture coord
						n=sscanf(line.c_str(),"vt %f %f",&vt.x,&vt.y);
						if(n!=2)
						{
							vt.x=0;
							vt.y=0;
							printf("failt!\n");
						}
						TextureCoord.push_back(vt);
					}
			break;

			case 'f':
				quad_temp.Set(-1);
				n=sscanf(line.c_str(),"f %d/%d/%d "
									"%d/%d/%d "
									"%d/%d/%d"
									"%d/%d/%d",
									&iv.x,&ivt.x,&ivn.x,
									&iv.y,&ivt.y,&ivn.y,
									&iv.z,&ivt.z,&ivn.z,
									&quad_temp.x,&quad_temp.y,&quad_temp.z
									);
				if(n==9){
					n=sscanf(line.c_str(),"f %d/%d/%d "
									"%d/%d/%d "
									"%d/%d/%d",
									&iv.x,&ivt.x,&ivn.x,
									&iv.y,&ivt.y,&ivn.y,
									&iv.z,&ivt.z,&ivn.z
									);
					triangle temp;
					temp.n[0]=ivn.x-1; temp.n[1]=ivn.y-1; temp.n[2]=ivn.z-1;
					temp.t[0]=ivt.x-1; temp.t[1]=ivt.y-1; temp.t[2]=ivt.z-1;
					temp.v[0]=iv.x-1; temp.v[1]=iv.y-1; temp.v[2]=iv.z-1;
					triangles.push_back(temp);
					indices.push_back(temp.v[0]);
					indices.push_back(temp.v[1]);
					indices.push_back(temp.v[2]);
				}
				else
				{
					n=sscanf(line.c_str(),"f %d/%d/%d "
									"%d/%d/%d "
									"%d/%d/%d"
									"%d/%d/%d",
									&iv.x,&ivt.x,&ivn.x,
									&iv.y,&ivt.y,&ivn.y,
									&iv.z,&ivt.z,&ivn.z,
									&quad_temp.x,&quad_temp.y,&quad_temp.z
									);
					triangle temp;
					temp.n[0]=ivn.x-1; temp.n[1]=ivn.y-1; temp.n[2]=ivn.z-1;
					temp.t[0]=ivt.x-1; temp.t[1]=ivt.y-1; temp.t[2]=ivt.z-1;
					temp.v[0]=iv.x-1; temp.v[1]=iv.y-1; temp.v[2]=iv.z-1;
					triangles.push_back(temp);
					indices.push_back(temp.v[0]);
					indices.push_back(temp.v[1]);
					indices.push_back(temp.v[2]);
					triangle temp1;
					temp1.n[0]=quad_temp.z-1; temp1.n[1]=ivn.x-1; temp1.n[2]=ivn.z-1;
					temp1.t[0]=quad_temp.y-1; temp1.t[1]=ivt.x-1; temp1.t[2]=ivt.z-1;
					temp1.v[0]=quad_temp.x-1; temp1.v[1]=iv.x-1; temp1.v[2]=iv.z-1;
					triangles.push_back(temp1);
					indices.push_back(temp1.v[0]);
					indices.push_back(temp1.v[1]);
					indices.push_back(temp1.v[2]);

				}

			break;
			
			default:

			break;
		}
	}
	file.close();

	printf("Vertex Normal num: %d %d\n",Vertices.size(),Normals.size());

}
void Geometry::loadBMP(std::string filename) 
{
	GLuint texture;
	int width, height;
	unsigned char * data;
	FILE * file;
	file = fopen(filename.c_str(), "rb");
	if (file == NULL) return;
	width = 1024;
	height = 512;
	data = (unsigned char *)malloc(width * height * 3);
	//int size = fseek(file,);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;

	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);


	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	return;

	/*
	#include <vector>
#include <fstream>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif


#ifdef _WIN32
#include <GL/gl.h>
#include <GL/glu.h>
#endif


typedef union PixelInfo
{
    std::uint32_t Colour;
    struct
    {
        std::uint8_t B, G, R, A;
    };
} *PPixelInfo;


class BMP
{
private:
    std::uint32_t width, height;
    std::uint16_t BitsPerPixel;
    std::vector<std::uint8_t> Pixels;

public:
    BMP(const char* FilePath);
    std::vector<std::uint8_t> GetPixels() const {return this->Pixels;}
    std::uint32_t GetWidth() const {return this->width;}
    std::uint32_t GetHeight() const {return this->height;}
    bool HasAlphaChannel() {return BitsPerPixel == 32;}
};

BMP::BMP(const char* FilePath)
{
    std::fstream hFile(FilePath, std::ios::in | std::ios::binary);
    if (!hFile.is_open()) throw std::invalid_argument("Error: File Not Found.");

    hFile.seekg(0, std::ios::end);
    std::size_t Length = hFile.tellg();
    hFile.seekg(0, std::ios::beg);
    std::vector<std::uint8_t> FileInfo(Length);
    hFile.read(reinterpret_cast<char*>(FileInfo.data()), 54);

    if(FileInfo[0] != 'B' && FileInfo[1] != 'M')
    {
        hFile.close();
        throw std::invalid_argument("Error: Invalid File Format. Bitmap Required.");
    }

    if (FileInfo[28] != 24 && FileInfo[28] != 32)
    {
        hFile.close();
        throw std::invalid_argument("Error: Invalid File Format. 24 or 32 bit Image Required.");
    }

    BitsPerPixel = FileInfo[28];
    width = FileInfo[18] + (FileInfo[19] << 8);
    height = FileInfo[22] + (FileInfo[23] << 8);
    std::uint32_t PixelsOffset = FileInfo[10] + (FileInfo[11] << 8);
    std::uint32_t size = ((width * BitsPerPixel + 31) / 32) * 4 * height;
    Pixels.resize(size);

    hFile.seekg (PixelsOffset, std::ios::beg);
    hFile.read(reinterpret_cast<char*>(Pixels.data()), size);
    hFile.close();
}

int main()
{
    BMP info = BMP("C:/Users/....../Desktop/SomeBmp.bmp");

    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, info.HasAlphaChannel() ? GL_RGBA : GL_RGB, info.GetWidth(), info.GetWidth(), 0, info.HasAlphaChannel() ? GL_BGRA : GL_BGR, GL_UNSIGNED_BYTE, info.GetPixels().data());
}
*********************************************


#include <vector>
#include <fstream>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif


#ifdef _WIN32
#include <GL/gl.h>
#include <GL/glu.h>
#endif

typedef union PixelInfo
{
std::uint32_t Colour;
struct
{
std::uint8_t R, G, B, A;
};
} *PPixelInfo;

class Tga
{
private:
std::vector<std::uint8_t> Pixels;
bool ImageCompressed;
std::uint32_t width, height, size, BitsPerPixel;

public:
Tga(const char* FilePath);
std::vector<std::uint8_t> GetPixels() {return this->Pixels;}
std::uint32_t GetWidth() const {return this->width;}
std::uint32_t GetHeight() const {return this->height;}
bool HasAlphaChannel() {return BitsPerPixel == 32;}
};

Tga::Tga(const char* FilePath)
{
std::fstream hFile(FilePath, std::ios::in | std::ios::binary);
if (!hFile.is_open()){throw std::invalid_argument("File Not Found.");}

std::uint8_t Header[18] = {0};
std::vector<std::uint8_t> ImageData;
static std::uint8_t DeCompressed[12] = {0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
static std::uint8_t IsCompressed[12] = {0x0, 0x0, 0xA, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

hFile.read(reinterpret_cast<char*>(&Header), sizeof(Header));

if (!std::memcmp(DeCompressed, &Header, sizeof(DeCompressed)))
{
BitsPerPixel = Header[16];
width  = Header[13] * 256 + Header[12];
height = Header[15] * 256 + Header[14];
size  = ((width * BitsPerPixel + 31) / 32) * 4 * height;

if ((BitsPerPixel != 24) && (BitsPerPixel != 32))
{
hFile.close();
throw std::invalid_argument("Invalid File Format. Required: 24 or 32 Bit Image.");
}

ImageData.resize(size);
ImageCompressed = false;
hFile.read(reinterpret_cast<char*>(ImageData.data()), size);
}
else if (!std::memcmp(IsCompressed, &Header, sizeof(IsCompressed)))
{
BitsPerPixel = Header[16];
width  = Header[13] * 256 + Header[12];
height = Header[15] * 256 + Header[14];
size  = ((width * BitsPerPixel + 31) / 32) * 4 * height;

if ((BitsPerPixel != 24) && (BitsPerPixel != 32))
{
hFile.close();
throw std::invalid_argument("Invalid File Format. Required: 24 or 32 Bit Image.");
}

PixelInfo Pixel = {0};
int CurrentByte = 0;
std::size_t CurrentPixel = 0;
ImageCompressed = true;
std::uint8_t ChunkHeader = {0};
int BytesPerPixel = (BitsPerPixel / 8);
ImageData.resize(width * height * sizeof(PixelInfo));

do
{
hFile.read(reinterpret_cast<char*>(&ChunkHeader), sizeof(ChunkHeader));

if(ChunkHeader < 128)
{
++ChunkHeader;
for(int I = 0; I < ChunkHeader; ++I, ++CurrentPixel)
{
hFile.read(reinterpret_cast<char*>(&Pixel), BytesPerPixel);

ImageData[CurrentByte++] = Pixel.B;
ImageData[CurrentByte++] = Pixel.G;
ImageData[CurrentByte++] = Pixel.R;
if (BitsPerPixel > 24) ImageData[CurrentByte++] = Pixel.A;
}
}
else
{
ChunkHeader -= 127;
hFile.read(reinterpret_cast<char*>(&Pixel), BytesPerPixel);

for(int I = 0; I < ChunkHeader; ++I, ++CurrentPixel)
{
ImageData[CurrentByte++] = Pixel.B;
ImageData[CurrentByte++] = Pixel.G;
ImageData[CurrentByte++] = Pixel.R;
if (BitsPerPixel > 24) ImageData[CurrentByte++] = Pixel.A;
}
}
} while(CurrentPixel < (width * height));
}
else
{
hFile.close();
throw std::invalid_argument("Invalid File Format. Required: 24 or 32 Bit TGA File.");
}

hFile.close();
this->Pixels = ImageData;
}


int main()
{
Tga info = Tga("C:/Users/...../Desktop/SomeTGA.tga");

GLuint texture = 0;
glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);
glTexImage2D(GL_TEXTURE_2D, 0, info.HasAlphaChannel() ? GL_RGBA : GL_RGB, info.GetWidth(), info.GetWidth(), 0, info.HasAlphaChannel() ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, info.GetPixels().data());

}

*/
}
void Geometry::Render()
{

	/*Vec3f v0,vn0;
	glBegin(GL_TRIANGLES);			
	for(int i=0;i<triangles.size();i++){
		for(int k=0;k<3;k++){
			v0=Vertices[triangles[i].v[k]];
			vn0=Normals[triangles[i].n[k]];
			glNormal3f(vn0.x,vn0.y,vn0.z);
			glVertex3f(v0.x,v0.y,v0.z);
		}

	}
	glEnd();
*/
	
	glGenVertexArrays(1,&vaoID);
	glBindVertexArray(vaoID);

	GLuint buffer[3];
	glGenBuffers(3, buffer);
	cout<<sizeof(vert_Buf[0])<<endl;
	cout<<sizeof(vert_Buf)<<endl;
	cout<<vert_Buf.size()<<endl;
	cout<<indices[0]<<endl;

	glBindBuffer(GL_ARRAY_BUFFER,buffer[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vert_Buf[0])*vert_Buf.size(),&vert_Buf[0],GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);                                  
	glEnableVertexAttribArray(0);
		cout<<"finish"<<endl;

	glBindBuffer(GL_ARRAY_BUFFER,buffer[1]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(norm_Buf[0])*norm_Buf.size(),&norm_Buf[0],GL_STATIC_DRAW);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffer[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices[0])*indices.size(),&indices[0],GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffer[2]);
	glPatchParameteri(GL_PATCH_VERTICES, 3);

	glDrawElements(GL_PATCHES,indices.size(),GL_UNSIGNED_INT,(void*)0);

	//float data[] = {0,0,1,1};
	//GLuint ubo;
	//glGenBuffers(1, &ubo);
	//glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	//glBufferData(GL_UNIFORM_BUFFER, sizeof(data)​, &data​, GL_STATIC_DRAW);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0); 

	////binding with binding point
	//glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);

}
