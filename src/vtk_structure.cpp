#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkStructuredGrid.h>
#include <vtkXMLStructuredGridWriter.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderWindow.h>
#include <vtkPolyData.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkCellArray.h>
#include <vtkHexahedron.h>
#include <vtkUnstructuredGrid.h>
#include <vtkTextMapper.h>
#include <vtkPolyhedron.h>
#include <vtkPyramid.h>
#include <vtkTetra.h>
#include <vtkWedge.h>
#include <vtkCellData.h>
#include <vtkTriangle.h>
#include <vtkArrayCalculator.h>
#include <vtkPolyDataMapper.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkAppendFilter.h>
#include <vtkOctreePointLocator.h>
#include <vtkFloatArray.h>

#include <vtkPointData.h>
#include <vtkVertexGlyphFilter.h>

#include "sys.h"
#include "grid.h"

// typedef struct {
//     int x_fil_count;
//     int y_fil_count;

//     filament *fil_array;
//     filament *fil_x_array;
//     filament *fil_y_array;
// } Fil;

int xfil_count(System *sys, Grid *grid)
{
    int count = 0;
    for(int i = 0; i < sys->fil_len; i++)
        if(sys->filVect[i]->begin_node.x - sys->filVect[i]->end_node.x != 0)
        count++;
    printf("\nTotal filaments in the X direction: %d\n", count);
    return count;
}

int yfil_count(System *sys, Grid *grid)
{
    int count = 0;
    for(int i = 0; i < sys->fil_len; i++)
        if(sys->filVect[i]->begin_node.y - sys->filVect[i]->end_node.y != 0)
        count++;
    printf("\nTotal filaments in the Y direction: %d\n", count);
    return count;
}

// void xy_filaments(System *sys, Fil *fil)
// {
//     fil->fil_x_array = (filament *)calloc(fil->x_fil_count, sizeof(filament));
//     fil->fil_y_array = (filament *)calloc(fil->y_fil_count, sizeof(filament));

//     int i;

//     int x_count = 0;
//     int y_count = 0;

//     for(i = 0; i < sys->fil_len; i++)
//     {
//         if(sys->fil_array[i].begin_node.x - sys->fil_array[i].end_node.x != 0)
//         {
//             fil->fil_x_array[x_count].layer = sys->fil_array[i].layer;
//             fil->fil_x_array[x_count].begin_node = sys->fil_array[i].begin_node;
//             fil->fil_x_array[x_count].end_node = sys->fil_array[i].end_node;
//             fil->fil_x_array[x_count].area = sys->fil_array[i].area;
//             fil->fil_x_array[x_count].height = sys->fil_array[i].height;
//             fil->fil_x_array[x_count].length = sys->fil_array[i].length;
//             fil->fil_x_array[x_count].num = sys->fil_array[i].num;
//             fil->fil_x_array[x_count].width = sys->fil_array[i].width;
//             fil->fil_x_array[x_count].stroom.x = sys->fil_array[i].stroom.x;
//             fil->fil_x_array[x_count].stroom.y = sys->fil_array[i].stroom.y;
//             fil->fil_x_array[x_count].stroom.z = sys->fil_array[i].stroom.z;

//             x_count++;
//         }

//         if(sys->fil_array[i].begin_node.y - sys->fil_array[i].end_node.y != 0)
//         {
//             fil->fil_y_array[y_count].layer = sys->fil_array[i].layer;
//             fil->fil_y_array[y_count].begin_node = sys->fil_array[i].begin_node;
//             fil->fil_y_array[y_count].end_node = sys->fil_array[i].end_node;
//             fil->fil_y_array[y_count].area = sys->fil_array[i].area;
//             fil->fil_y_array[y_count].height = sys->fil_array[i].height;
//             fil->fil_y_array[y_count].length = sys->fil_array[i].length;
//             fil->fil_y_array[y_count].num = sys->fil_array[i].num;
//             fil->fil_y_array[y_count].width = sys->fil_array[i].width;
//             fil->fil_y_array[y_count].stroom.x = sys->fil_array[i].stroom.x;
//             fil->fil_y_array[y_count].stroom.y = sys->fil_array[i].stroom.y;
//             fil->fil_y_array[y_count].stroom.z = sys->fil_array[i].stroom.z;

//             y_count++;
//         }
//     }
// }

void get_x_filament(System *sys, Node *corners, int i)
{
    int const_height = 1;

    // if (strcmp(sys->filVect[i].layer, "m0") != 0)
    //     const_height = sys->z_fil_height;

    // double w = sys->fil_array[i].width;
    // double h = sys->fil_array[i].height;

    // double x_front_point = sys->fil_array[i].end_node.x;
    // double y_front_point = sys->fil_array[i].end_node.y;
    // double z_front_point = sys->fil_array[i].end_node.z;

    // double x_back_point = sys->fil_array[i].begin_node.x;
    // double y_back_point = sys->fil_array[i].begin_node.y;
    // double z_back_point = sys->fil_array[i].begin_node.z;

    double w = sys->filVect[i]->width;
    double h = sys->filVect[i]->height;

    double x_front_point = sys->filVect[i]->end_node.x;
    double y_front_point = sys->filVect[i]->end_node.y;
    double z_front_point = sys->filVect[i]->end_node.z;

    double x_back_point = sys->filVect[i]->begin_node.x;
    double y_back_point = sys->filVect[i]->begin_node.y;
    double z_back_point = sys->filVect[i]->begin_node.z;

    // Node orientation is defined in VTK manual, page 9.
    if(sys->filVect[i]->begin_node.x - sys->filVect[i]->end_node.x != 0) {
        // Corner 0
        corners[0].x = x_front_point;
        corners[0].y = y_front_point + w;
        corners[0].z = z_front_point - h;

        // Corner 1
        corners[1].x = x_back_point;
        corners[1].y = y_back_point + w;
        corners[1].z = z_back_point - h;

        // Corner 2
        corners[3].x = x_front_point;
        corners[3].y = y_front_point - w;
        corners[3].z = z_front_point - h;

        // Corner 3
        corners[2].x = x_back_point;
        corners[2].y = y_back_point - w;
        corners[2].z = z_back_point - h;

        // Corner 4
        corners[4].x = x_front_point;
        corners[4].y = y_front_point + w;
        corners[4].z = z_front_point + const_height * h;

        // Corner 5
        corners[5].x = x_back_point;
        corners[5].y = y_back_point + w;
        corners[5].z = z_back_point + const_height * h;

        // Corner 6
        corners[7].x = x_front_point;
        corners[7].y = y_front_point - w;
        corners[7].z = z_front_point + const_height * h;

        // Corner 7
        corners[6].x = x_back_point;
        corners[6].y = y_back_point - w;
        corners[6].z = z_back_point + const_height * h;
    }

    // Node orientation is defined in VTK manual, page 9.
    // if(sys->filVect[i]->begin_node.y - sys->filVect[i]->end_node.y != 0) {
    //     // Corner 0
    //     corners[0].x = x_front_point - w;
    //     corners[0].y = y_front_point;
    //     corners[0].z = z_front_point - h;

    //     // Corner 1
    //     corners[1].x = x_front_point + w;
    //     corners[1].y = y_front_point;
    //     corners[1].z = z_front_point - h;

    //     // Corner 2
    //     corners[2].x = x_back_point + w;
    //     corners[2].y = y_back_point;
    //     corners[2].z = z_back_point - h;

    //     // Corner 3
    //     corners[3].x = x_back_point - w;
    //     corners[3].y = y_back_point;
    //     corners[3].z = z_back_point - h;

    //     // Corner 4
    //     corners[4].x = x_front_point - w;
    //     corners[4].y = y_front_point;
    //     corners[4].z = z_front_point + const_height * h;

    //     // Corner 5
    //     corners[5].x = x_front_point + w;
    //     corners[5].y = y_front_point;
    //     corners[5].z = z_front_point + const_height * h;

    //     // Corner 6
    //     corners[6].x = x_back_point + w;
    //     corners[6].y = y_back_point;
    //     corners[6].z = z_back_point + const_height * h;

    //     // Corner 7
    //     corners[7].x = x_back_point - w;
    //     corners[7].y = y_back_point;
    //     corners[7].z = z_back_point + const_height * h;
    // }
}

void vtk_x_filaments(System *sys, Grid *grid) {
    std::cout << "Start writing VTK file..." << std::endl;

    vtkSmartPointer<vtkUnstructuredGrid> uGridTwo = vtkSmartPointer<vtkUnstructuredGrid>::New();
    vtkSmartPointer<vtkPoints> pointsTwo = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> hexTwo = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkFloatArray> x_curr = vtkSmartPointer<vtkFloatArray>::New();
    
    Node *corners_segment = (Node *)calloc(8, sizeof(Node));

    for(int i = 0; i < sys->filVect; i++) {
        x_curr->SetNumberOfComponents(3);
        x_curr->SetName("x_curr");

        for (int j = 0; j < 8; j++) {
            double px = fil->corners[j].x;
            double py = fil->corners[j].y;
            double pz = fil->corners[j].z;
            
            pointsTwo->InsertNextPoint(px, py, pz);
        }

        float cx = sys->filVect[i]->stroom.x;
        float cy = sys->filVect[i]->stroom.y;
        float cz = sys->filVect[i]->stroom.z;

        for (int j = 0; j < 8; j++) {
            float vertex[3] = {cx, cy, cz};
            x_curr->InsertNextTupleValue(vertex);
        }

        vtkSmartPointer<vtkHexahedron> hex = vtkSmartPointer<vtkHexahedron>::New();

        for (int j = 0; j < 8; j++)
            hex->GetPointIds()->SetId(j, 8*i + j);

        hexTwo->InsertNextCell(hex);
    }

    printf("Finished Calculating\n");

    uGridTwo->SetPoints(pointsTwo);
    uGridTwo->GetPointData()->SetVectors(x_curr);
    uGridTwo->SetCells(12, hexTwo);

    vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    
    writer->SetFileName("vtk_x_curr.vtu");    
    writer->SetInputData(uGridTwo);
    writer->Write();

    std::cout << "Finished writing VTK file" << std::endl;
}

void vtk_y_filaments(System *sys, Grid *grid) {
    std::cout << "Start writing VTK file..." << std::endl;

    vtkSmartPointer<vtkUnstructuredGrid> uGridTwo = vtkSmartPointer<vtkUnstructuredGrid>::New();
    vtkSmartPointer<vtkPoints> pointsTwo = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> hexTwo = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkFloatArray> y_curr = vtkSmartPointer<vtkFloatArray>::New();

    int yfilnum = yfil_count(sys, grid);
    
    Node *corners_segment = (Node *)calloc(8, sizeof(Node));

    for(int i = 0; i < yfilnum; i++) {
        get_x_filament(sys, corners_segment, i);

        y_curr->SetNumberOfComponents(3);
        y_curr->SetName("y_curr");

        for (int j = 0; j < 8; j++)
            pointsTwo->InsertNextPoint(corners_segment[j].x, corners_segment[j].y, -corners_segment[j].z);

        float x = sys->filVect[i]->stroom.x;
        float y = sys->filVect[i]->stroom.y;
        float z = sys->filVect[i]->stroom.z;

        for (int j = 0; j < 8; j++) {
            float vertex[3] = {x, y, z};
            y_curr->InsertNextTupleValue(vertex);
        }

        vtkSmartPointer<vtkHexahedron> hex = vtkSmartPointer<vtkHexahedron>::New();

        for (int j = 0; j < 8; j++)
            hex->GetPointIds()->SetId(j, 8*i + j);

        hexTwo->InsertNextCell(hex);
    }

    printf("Finished Calculating\n");

    uGridTwo->SetPoints(pointsTwo);
    uGridTwo->GetPointData()->SetVectors(y_curr);
    uGridTwo->SetCells(12, hexTwo);

    vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    
    writer->SetFileName("vtk_y_curr.vtu");    
    writer->SetInputData(uGridTwo);
    writer->Write();

    std::cout << "Finished writing VTK file" << std::endl;
}
    

void structured_grid(System *sys, Grid *grid) {

    std::cout << "Start writing VTK file..." << std::endl;

    vtkSmartPointer<vtkUnstructuredGrid> uGridTwo = vtkSmartPointer<vtkUnstructuredGrid>::New();
    vtkSmartPointer<vtkPoints> pointsTwo = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> hexTwo = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkFloatArray> b_field = vtkSmartPointer<vtkFloatArray>::New();

    // Node *corners_segment = (Node *)calloc(8, sizeof(Node));

    // for(int i = 0; i < sys->fil_len; i++) {
    //     get_corner_coords_segments(sys, corners_segment, i);

    //     b_field->SetNumberOfComponents(3);
    //     b_field->SetName("current_field");

    //     for (int j = 0; j < 8; j++)
    //         pointsTwo->InsertNextPoint(corners_segment[j].x, corners_segment[j].y, -corners_segment[j].z);

    //     float x = sys->filVect[i]->stroom(0);
    //     float y = sys->filVect[i]->stroom(1);
    //     float z = sys->filVect[i]->stroom(2);

    //     for (int j = 0; j < 8; j++) {
    //         float vertex[3] = {x, y, z};
    //         b_field->InsertNextTupleValue(vertex);
    //     }

    //     vtkSmartPointer<vtkHexahedron> hex = vtkSmartPointer<vtkHexahedron>::New();

    //     for (int j = 0; j < 8; j++)
    //         hex->GetPointIds()->SetId(j, 8*i + j);

    //     hexTwo->InsertNextCell(hex);
    // }

    // printf("Finished Calculating\n");

    // uGridTwo->SetPoints(pointsTwo);
    // uGridTwo->GetPointData()->SetVectors(b_field);
    // uGridTwo->SetCells(12, hexTwo);

    // vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    
    // writer->SetFileName("structure.vtu");    
    // writer->SetInputData(uGridTwo);
    // writer->Write();

    std::cout << "Finished writing VTK file" << std::endl;
}























