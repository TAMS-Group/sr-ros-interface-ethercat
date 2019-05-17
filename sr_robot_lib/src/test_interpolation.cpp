#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <random>
#include <sstream>
#include <string>
using std::string;
#include "sr_robot_lib/pwl_interp_2d_scattered.hpp"
#include "sr_robot_lib/shepard_interp_2d.hpp"


  int element_neighbor[3*2*25];
  int element_num;
  int element_order = 3;
  int ni = 1;
  int node_num = 25;
  //raw J1, raw J2
  double node_xy[2*25] = {
    2738, 2301,
    2693, 2154,
    2680, 1978,
    2677, 1840,
    2664, 1707,
    2334, 2287,
    2242, 2095,
    2230, 1953,
    2223, 1807,
    2206, 1685,
    1839, 2243,
    1772, 2112,
    1764, 1928,
    1755, 1762,
    1683, 1650,
    1387, 2219,
    1375, 2056,
    1370, 1884,
    1337, 1741,
    1329, 1630,
    1141, 2206,
    1132, 2055,
    1114, 1877,
    1103, 1730,
    1092, 1615
    };
  int triangle[3*2*25];
  //Sample coordinates
  double xyi[2*1];
  double zd_thj1[25] = {
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    // -0.5,
    // -0.5,
    // -0.5,
    // -0.5,
    // -0.5,
    0.3927,
    0.3927,
    0.3927,
    0.3927,
    0.3927,
    0.7854,
    0.7854,
    0.7854,
    0.7854,
    0.7854,
    1.1781,
    1.1781,
    1.1781,
    1.1781,
    1.1781,
    1.5708,
    1.5708,
    1.5708,
    1.5708,
    1.5708
  };
  double zd_thj2[25] = {
    0.6981,
    0.34906,
    0.0,
    -0.34906,
    -0.6981,
    0.6981,
    0.34906,
    0.0,
    -0.34906,
    -0.6981,
    0.6981,
    0.34906,
    0.0,
    -0.34906,
    -0.6981,
    0.6981,
    0.34906,
    0.0,
    -0.34906,
    -0.6981,
    0.6981,
    0.34906,
    0.0,
    -0.34906,
    -0.6981
  };
  double *zi;

  double xd[25] = {
    2738,
    2693,
    2680,
    2677,
    2664,
    2334,
    2242,
    2230,
    2223,
    2206,
    1839,
    1772,
    1764,
    1755,
    1683,
    1387,
    1375,
    1370,
    1337,
    1329,
    1141,
    1132,
    1114,
    1103,
    1092
    };

   double yd[25] = {
    2301,
    2154,
    1978,
    1840,
    1707,
    2287,
    2095,
    1953,
    1807,
    1685,
    2243,
    2112,
    1928,
    1762,
    1650,
    2219,
    2056,
    1884,
    1741,
    1630,
    2206,
    2055,
    1877,
    1730,
    1615
    }; 

void print_double_array(double *data, int length, int stride, int start_at)
{
  std::stringstream ss;
  ss << "[";
  for(int i=start_at; i<length; i=i+stride)
  {
    ss << data[i];
    if ((i / stride) * stride < (length - stride))
    {
      ss << ",";
    }
  }
  ss << "]";
  std::cout << ss.str() << std::endl;
}

int main(int argc, char** argv)
{
  int nb_samples = 1000;
  // Print data points
  print_double_array(node_xy, 50, 2, 0);
  print_double_array(node_xy, 50, 2, 1);
  print_double_array(zd_thj1, 25, 1, 0);
  print_double_array(zd_thj2, 25, 1, 0);
  //
  //  Set up the Delaunay triangulation.
  //
    r8tris2 ( node_num, node_xy, element_num, triangle, element_neighbor );

    for ( int j = 0; j < element_num; j++ )
    {
      for ( int i = 0; i < 3; i++ )
      {
        if ( 0 < element_neighbor[i+j*3] )
        {
          element_neighbor[i+j*3] = element_neighbor[i+j*3] - 1;
        }
      }
    }

    triangulation_order3_print ( node_num, element_num, node_xy, triangle, element_neighbor );

  std::stringstream log_triangles;
  log_triangles << "triangles = [";
  for(int i=0; i<element_num; i++)
  {
    log_triangles << "[";
    for(int j=0; j<3; j++)
    {
      log_triangles << triangle[i * 3 + j] << ",";
    }
    log_triangles << "],";
  }
  log_triangles << "]";
  std::cout << log_triangles.str() << std::endl;

  std::stringstream log_x, log_y, log_z;
  int raw_1;
  int raw_2;
  log_x << "x = [";
  log_y << "y = [";
  log_z << "z = [";
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution_x(1000,2800);
  std::uniform_int_distribution<int> distribution_y(1500,2400);


  // Delaunay triangulation interpolation
  using namespace std::chrono;
  high_resolution_clock::time_point t1 = high_resolution_clock::now();

  for(int i=0; i<nb_samples; i++)
  {
    raw_1 = distribution_x(generator);
    raw_2 = distribution_y(generator);
    xyi[0] = static_cast<double> (raw_1);
    xyi[1] = static_cast<double> (raw_2);
    log_x << xyi[0] << ",";
    log_y << xyi[1] << ",";
    zi = pwl_interp_2d_scattered_value (node_num, node_xy, zd_thj1, element_num,
                                              triangle, element_neighbor, ni, xyi);
    log_z << zi[0] << ",";
    // ROS_INFO("THJ1: %f THJ2: %f Interpolated THJ1: %f", this->xyi[0], this->xyi[1], tmp_cal_value);
    delete [] zi;
  }

  high_resolution_clock::time_point t2 = high_resolution_clock::now();

  duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

  std::cout << "It took " << (time_span.count() * 1000000) << " us.";
  std::cout << std::endl;

  log_x << "]";
  log_y << "]";
  log_z << "]";
  std::cout << log_x.str() << std::endl << std::endl;
  std::cout << log_y.str() << std::endl << std::endl;
  std::cout << log_z.str() << std::endl << std::endl;




  // Shepard interpolation
  log_x.str(std::string());
  log_y.str(std::string());
  log_z.str(std::string());
  log_x << "x = [";
  log_y << "y = [";
  log_z << "z = [";
  t1 = high_resolution_clock::now();
  // the power used in the distance weighting
  double p = 4.0;

  for(int i=0; i<nb_samples; i++)
  {
    raw_1 = distribution_x(generator);
    raw_2 = distribution_y(generator);
    xyi[0] = static_cast<double> (raw_1);
    xyi[1] = static_cast<double> (raw_2);
    log_x << xyi[0] << ",";
    log_y << xyi[1] << ",";

    zi = shepard_interp_2d ( node_num, xd, yd, zd_thj1, p, ni, &(xyi[0]), &(xyi[1]) );
    log_z << zi[0] << ",";
    // ROS_INFO("THJ1: %f THJ2: %f Interpolated THJ1: %f", this->xyi[0], this->xyi[1], tmp_cal_value);
    delete [] zi;
  }

  t2 = high_resolution_clock::now();

  time_span = duration_cast<duration<double>>(t2 - t1);

  std::cout << "Shepard took " << (time_span.count() * 1000000) << " us.";
  std::cout << std::endl;

  log_x << "]";
  log_y << "]";
  log_z << "]";
  std::cout << log_x.str() << std::endl << std::endl;
  std::cout << log_y.str() << std::endl << std::endl;
  std::cout << log_z.str() << std::endl << std::endl;

  return 0;
}