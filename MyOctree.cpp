/*************************************************

Copyright: Weixing.Wang

Author: ������

Date:2021-12-20

Description:�˳�����Ҫ����PCD�ĵ��ƣ����а˲���ѹ����
			Ȼ�����delaunay���ǻ������ͨ��OpenGL��ʾ

**************************************************/


// MyOctree.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include<iostream>
#include <pcl/visualization/pcl_visualizer.h>
#include<pcl/io/pcd_io.h>
#include<string.h>
#include <direct.h>

#include "stdafx.h"
#include "Point3D.h"
#include "Octree.h"
#include "Triangle.h"

using namespace std;
using namespace pcl;

using namespace cv;
int _tmain(int argc, _TCHAR* argv[])
{
	COctree octree;
	vector<CPoint3f> points;
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("Compression_Trianglization"));
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_sample(new pcl::PointCloud<pcl::PointXYZ>());
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloudIn(new pcl::PointCloud<pcl::PointXYZ>());


	//�ļ���ַ
	char filein[] = "D:\\Down\\rabbit.pcd";
	char fileout[] = "D:\\Down\\project_compressed.pcd";
	char normalOut[] = "D:\\Down\\compression_normals.pcd";
	string s = filein;

	//���ô���
	int v1(0);
	int v2(1);
	int v3(2);
	int v4(3);
	viewer->createViewPort(0.0, 0.5, 0.5, 1.0, v1); //(Xmin,Ymin,Xmax,Ymax)���ô�������
	viewer->createViewPort(0.5, 0.5, 1.0, 1.0, v2);
	viewer->createViewPort(0.0, 0, 0.5, 0.5, v3);
	viewer->createViewPort(0.5, 0.0, 1, 0.5, v4);


	viewer->setBackgroundColor(0, 0, 0, v1);
	viewer->setBackgroundColor(0, 0, 255, v2);
	viewer->setBackgroundColor(255, 0, 0, v3);
	viewer->setBackgroundColor(200, 200, 200, v4);


	viewer->addText("original", 10, 10, 16, 1, 1, 1, "v1 text", v1);//�����ӿ�����
	viewer->addText("OCTREE sampled", 10, 10, 16, 1, 1, 1, "v2 text", v2);
	viewer->addText("Trianglizaed", 10, 10, 16, 1, 1, 1, "v3 text", v3);
	viewer->addText("Coded By Weixing.Wang", 20, 210, 24, 0, 0, 0, "v4 text1", v4);
	viewer->addText("email:1474446236@qq.com", 20, 160, 24, 0, 0, 0, "v4 text2", v4);
	viewer->addText("github:https://github.com/wwx1474446236", 20, 100, 24, 0, 0, 0, "v4 text4", v4);
	viewer->addText("Copyright (c) Weixing.Wang in Nov. 20th 2021", 20, 50, 24, 0, 0, 0, "v4 text3", v4);
	viewer->addText("Only for CPU ~(-o~-)  (QAQ)", 20, 340, 28, 0, 0, 0, "v4 text5", v4);
	viewer->addText("Donot expect version2.0!!!, I will not touch this code any more!!!", 20, 440, 28, 0, 0, 0, "v4 text6", v4);

	//ѹ��
	octree.initTree();
	octree.ReadFile(filein, points);
	octree.buildTree(points, 10);
	octree.egordicNode(0, 1);
	cout << "*********ѹ�����**********\n";
	cout << "�ļ��� ";
	octree.WriteFile(fileout);
	cout << "========>\n";

	int size = octree.getSize();
	float *X = new float[size];
	float *Y = new float[size];
	float *Z = new float[size];
	octree.outArray(X, Y, Z);


	// delaunay ���ǻ�
	cout << "========>���ڽ���delaunay���ǻ�\n";
	pcl::PolygonMesh triangles = Triangle(X, Y, Z, size, cloud_sample, normalOut);
	cout << "*******����������ϣ���ͼ�ν��������ɣ���*********\n";
	pcl::io::loadPCDFile(s, *cloudIn);



	//��ʾ����
	viewer->addPointCloud(cloudIn, "cloudIn", v1);
	viewer->addPointCloud(cloud_sample, "cloud_sample", v2);
	viewer->addPolygonMesh(triangles, "wangge", v3);

	while (!viewer->wasStopped())
	{
		viewer->spinOnce();
	}

	return 0;
}