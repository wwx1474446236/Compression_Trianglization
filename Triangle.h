#pragma once
#include<iostream>
#include<pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/io/obj_io.h>
#include<pcl/point_types.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>

using namespace std;
using namespace pcl;

pcl::PolygonMesh Triangle(float *X, float *Y, float *Z, int size, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud1, char *filename) {
	/*pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>());*/
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_smoothed(new pcl::PointCloud<pcl::PointXYZ>());
	//PointCloud<PointXYZ> cloud1;
	//��ȫpcl������Ϣ
	cloud1->width = size;
	cloud1->height = 1;
	cloud1->is_dense = false;
	cloud1->points.resize(cloud1->width * cloud1->height);
	for (int i = 0; i < size; i++) {
		cloud1->points[i].x = X[i];
		cloud1->points[i].y = Y[i];
		cloud1->points[i].z = Z[i];
	}
	cloud_smoothed = cloud1;


	// Normal estimation*  �������
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;//���÷��߹��ƶ���
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);//�洢���Ƶķ���
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree1(new pcl::search::KdTree<pcl::PointXYZ>);//����kd��ָ��
	tree1->setInputCloud(cloud_smoothed);//��cloud_smoothed����tree����
	n.setInputCloud(cloud_smoothed);//Ϊ���߹��ƶ��������������
	n.setSearchMethod(tree1);//������������
	n.setKSearch(30);//����k�������ص�������Χ
	n.compute(*normals);//���Ʒ��ߴ洢�����normals


	//* normals should not contain the point normals + surface curvatures
	// �������
	std::cout << "normals: " << normals->size() << ", " << "normals fields: " << pcl::getFieldsList(*normals) << std::endl;
	pcl::io::savePCDFileASCII(filename, *normals);


	// Concatenate the XYZ and normal fields*
	pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals(new pcl::PointCloud<pcl::PointNormal>);//
	pcl::concatenateFields(*cloud_smoothed, *normals, *cloud_with_normals);//�����ֶΣ�cloud_with_normals�洢�������
	//* cloud_with_normals = cloud + normals
	std::cout << "cloud_with_normals: " << cloud_with_normals->size() << endl;
	// Create search tree*
	pcl::search::KdTree<pcl::PointNormal>::Ptr tree2(new pcl::search::KdTree<pcl::PointNormal>);//��������������
	tree2->setInputCloud(cloud_with_normals);//����������ƹ���tree


	// Initialize objects
	pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;//�������ǻ�����
	pcl::PolygonMesh triangles;//�洢�������ǻ�������ģ��

	// Set the maximum distance between connected points (maximum edge length)
	gp3.setSearchRadius(30);         //���������뾶radius����ȷ�����ǻ�ʱkһ�ڽ�����뾶��

	// Set typical values for the parameters
	gp3.setMu(3);                     //���������㵽����������ĳ˻�ϵ�� mu �����ÿ�������������������룬����ʹ���㷨����Ӧ�����ܶȵı仯
	gp3.setMaximumNearestNeighbors(100);//����������������������������Ŀ100 ��
	gp3.setMaximumSurfaceAngle(M_PI / 2);  //45 degrees����������ʱ�����Ƕ� eps_angle ����ĳ�㷨������ڲ�����ķ���ƫ��Ƕȳ��������Ƕ�ʱ������ʱ�Ͳ����Ǹõ㡣
	gp3.setMinimumAngle(M_PI / 36);        //10 degrees���������ǻ��������ε���С�ǣ����� minimum_angle Ϊ��С�ǵ�ֵ��
	gp3.setMaximumAngle(5 * M_PI / 6);    //120 degrees���������ǻ��������ε����ǣ����� maximum_angle Ϊ���ǵ�ֵ��
	gp3.setNormalConsistency(false);     //����һ����־ consistent ������֤���߳���һ�£��������Ϊ true ���ʹ���㷨���ַ��߷���һ�£����Ϊ false �㷨�򲻻���з���һ���Լ�顣

	// Get result
	gp3.setInputCloud(cloud_with_normals);//�����������Ϊ�������
	gp3.setSearchMethod(tree2);           //����������ʽtree2
	gp3.reconstruct(triangles);
	return triangles;
}
