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
	//补全pcl点云信息
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


	// Normal estimation*  法向估计
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;//设置法线估计对象
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);//存储估计的法线
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree1(new pcl::search::KdTree<pcl::PointXYZ>);//定义kd树指针
	tree1->setInputCloud(cloud_smoothed);//用cloud_smoothed构造tree对象
	n.setInputCloud(cloud_smoothed);//为法线估计对象设置输入点云
	n.setSearchMethod(tree1);//设置搜索方法
	n.setKSearch(30);//设置k邻域搜素的搜索范围
	n.compute(*normals);//估计法线存储结果到normals


	//* normals should not contain the point normals + surface curvatures
	// 输出法线
	std::cout << "normals: " << normals->size() << ", " << "normals fields: " << pcl::getFieldsList(*normals) << std::endl;
	pcl::io::savePCDFileASCII(filename, *normals);


	// Concatenate the XYZ and normal fields*
	pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals(new pcl::PointCloud<pcl::PointNormal>);//
	pcl::concatenateFields(*cloud_smoothed, *normals, *cloud_with_normals);//连接字段，cloud_with_normals存储有向点云
	//* cloud_with_normals = cloud + normals
	std::cout << "cloud_with_normals: " << cloud_with_normals->size() << endl;
	// Create search tree*
	pcl::search::KdTree<pcl::PointNormal>::Ptr tree2(new pcl::search::KdTree<pcl::PointNormal>);//定义搜索树对象
	tree2->setInputCloud(cloud_with_normals);//利用有向点云构造tree


	// Initialize objects
	pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;//定义三角化对象
	pcl::PolygonMesh triangles;//存储最终三角化的网络模型

	// Set the maximum distance between connected points (maximum edge length)
	gp3.setSearchRadius(30);         //设置搜索半径radius，来确定三角化时k一邻近的球半径。

	// Set typical values for the parameters
	gp3.setMu(3);                     //设置样本点到最近邻域距离的乘积系数 mu 来获得每个样本点的最大搜索距离，这样使得算法自适应点云密度的变化
	gp3.setMaximumNearestNeighbors(100);//设置样本点最多可以搜索的邻域数目100 。
	gp3.setMaximumSurfaceAngle(M_PI / 2);  //45 degrees，设置连接时的最大角度 eps_angle ，当某点法线相对于采样点的法线偏离角度超过该最大角度时，连接时就不考虑该点。
	gp3.setMinimumAngle(M_PI / 36);        //10 degrees，设置三角化后三角形的最小角，参数 minimum_angle 为最小角的值。
	gp3.setMaximumAngle(5 * M_PI / 6);    //120 degrees，设置三角化后三角形的最大角，参数 maximum_angle 为最大角的值。
	gp3.setNormalConsistency(false);     //设置一个标志 consistent ，来保证法线朝向一致，如果设置为 true 则会使得算法保持法线方向一致，如果为 false 算法则不会进行法线一致性检查。

	// Get result
	gp3.setInputCloud(cloud_with_normals);//设置输入点云为有向点云
	gp3.setSearchMethod(tree2);           //设置搜索方式tree2
	gp3.reconstruct(triangles);
	return triangles;
}
