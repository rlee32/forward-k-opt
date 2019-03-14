#include "LengthMap.h"
#include "Tour.h"
#include "fileio.h"
#include "point_quadtree/Domain.h"
#include "point_quadtree/Node.h"
#include "point_quadtree/morton_keys.h"
#include "point_quadtree/point_quadtree.h"
#include "solver.h"

#include <iostream>

int main(int argc, const char** argv)
{
    if (argc < 2)
    {
        std::cout << "Arguments: point_set_file_path optional_tour_file_path" << std::endl;
        return 0;
    }

    // Read input files.
    const auto coordinates {fileio::read_coordinates(argv[1])};
    const auto& x {coordinates[0]};
    const auto& y {coordinates[1]};
    const auto initial_tour = fileio::initial_tour(argc, argv, x.size());

    // Distance calculation.
    point_quadtree::Domain domain(x, y);
    LengthMap length_map(x, y);
    Tour tour(&domain, initial_tour, &length_map);
    std::cout << "Initial tour length: " << tour.length() << std::endl;

    // Quad tree.
    const auto morton_keys
        {point_quadtree::morton_keys::compute_point_morton_keys(x, y, domain)};
    Box box;
    box.xmin = domain.xmin();
    box.ymin = domain.ymin();
    box.xmax = domain.xmin() + domain.xdim(0);
    box.ymax = domain.ymin() + domain.ydim(0);
    point_quadtree::Node root(box);
    point_quadtree::initialize_points(root, morton_keys, domain);


    bool improved {false};
    do
    {
        improved = false;

        auto swap = solver::find_forward_swap(tour, root);
        if (not swap.empty())
        {
            tour.forward_swap(swap, false);
            improved = true;
        }

        swap = solver::find_forward_swap_ab(tour, root);
        if (not swap.empty())
        {
            tour.forward_swap(swap, true);
            improved = true;
        }
    } while (improved);
    std::cout << "final length: " << tour.length() << std::endl;
    tour.validate();
    return 0;
}
