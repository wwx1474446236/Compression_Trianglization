#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
int main() {
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloudIn(new pcl::PointCloud<pcl::PointXYZ>());

	pcl::io::loadPCDFile("E:\\rabbit.pcd", *cloudIn);
	
	pcl::visualization::CloudViewer viewer("cloud viewer");
	viewer.showCloud(cloudIn);

	while (!viewer.wasStopped()) {
	}
	system("pause");
	return 0;
}