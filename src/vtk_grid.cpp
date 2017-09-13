#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkXMLStructuredGridWriter.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkXMLPolyDataReader.h>

#include <vtkPolyDataMapper.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkArrayCalculator.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkAppendFilter.h>
#include <vtkPolygon.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkProperty.h>
#include <vtkStructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyhedron.h>
#include <vtkWedge.h>
#include <vtkPointSource.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPolyDataReader.h>

#include "sys.h"
#include "grid.h"

using namespace ClipperLib;

// For compatibility with new VTK generic data arrays
#ifdef vtkGenericDataArray_h
#define InsertNextTupleValue InsertNextTypedTuple
#endif

bool point_inside_layer(double x, double y, Paths layerpoly) 
{
    bool inside_layer = false;

    for (auto poly : layerpoly) {
        int inside = PointInPolygon(IntPoint(x, y), poly);

        if (inside == 1)
            inside_layer = true;
    }

    return inside_layer;
}

void create_layers(System *sys, vtkSmartPointer<vtkCellArray> polygons, vtkSmartPointer<vtkPoints> points) 
{
    int polynumber = 0;
    
    for (auto poly : sys->layerpoly) {
        std::cout << "\n--- Polygon layer ---" << std::endl;
        std::cout << sys->layerpoly << std::endl;
        int nodenum = 0;

        vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
        polygon->GetPointIds()->SetNumberOfIds(poly.size());
        
        std::cout << "--- Polygon nodes ---" << std::endl;
        for (auto node : poly) {
            std::cout << node.X << " " << node.Y << std::endl;
            points->InsertNextPoint(node.X, node.Y, 0.0);

            polygon->GetPointIds()->SetId(nodenum, poly.size()*polynumber + nodenum);
            nodenum++;
        }

        polygons->InsertNextCell(polygon);
        polynumber++;
    }
}

void create_grid(System *sys, Grid *grid, vtkSmartPointer<vtkCellArray> polygons, vtkSmartPointer<vtkPoints> points) 
{
    int count = 0;
    
    for (int i = 0; i < sys->NGrid; i++) {
        for (int j = 0; j < sys->NGrid; j++) {            
            vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
            polygon->GetPointIds()->SetNumberOfIds(4);
            
            double x = grid->nx(i,j);
            double y = grid->ny(i,j);
            double z = grid->nz(i,j);

            if (!point_inside_layer(x, y, sys->layerpoly)) {
                points->InsertNextPoint(x, y, z);
                points->InsertNextPoint(x+2e5, y, z);
                points->InsertNextPoint(x+2e5, y+2e5, z);
                points->InsertNextPoint(x, y+2e5, z);
        
                polygon->GetPointIds()->SetId(0, 4*count + 0);
                polygon->GetPointIds()->SetId(1, 4*count + 1);
                polygon->GetPointIds()->SetId(2, 4*count + 2);
                polygon->GetPointIds()->SetId(3, 4*count + 3);
                
                polygons->InsertNextCell(polygon);
                count++;
            }
        }
    }
}

void vtk_grid_polygons(System *sys, Grid *grid) 
{
    std::cout << "\n--- Creating VTK grid polygons ---" << std::endl;
    
    vtkSmartPointer<vtkCellArray> polygons = vtkSmartPointer<vtkCellArray>::New();    
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    
    create_layers(sys, polygons, points);
    // create_grid(sys, grid, polygons, points);

    vtkSmartPointer<vtkPolyData> polygonPolyData = vtkSmartPointer<vtkPolyData>::New();
    polygonPolyData->SetPoints(points);
    polygonPolyData->SetPolys(polygons);

    vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    writer->SetFileName("magix_grid_polygons.vtp");
    writer->SetInputData(polygonPolyData);
    writer->Write();

    std::cout << "\n--- VTK grid finished polygons ---" << std::endl;    
}

void vtk_hfield_struct_grid(System *sys, Grid *grid) 
{
    vtkSmartPointer<vtkDoubleArray> hfield = vtkSmartPointer<vtkDoubleArray>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkStructuredGrid> structuredGrid = vtkSmartPointer<vtkStructuredGrid>::New();

    hfield->SetNumberOfComponents(3);
    hfield->SetName("hfield");

    for (int i = 0; i < sys->NGrid; i++) {
        for (int j = 0; j < sys->NGrid; j++) {
            double r = grid->Hx(i,j);
            double g = grid->Hy(i,j);
            double b = grid->Hz(i,j);

            // r = (float) image(i,j)(0);
            // g = (float) image(i,j)(1);
            // b = (float) image(i,j)(2);

            double vertex[3] = {r, g, b};

            double x = grid->nx(i,j);
            double y = grid->ny(i,j);
            double z = grid->nz(i,j);

            points->InsertNextPoint(x, y, z);
            hfield->InsertNextTupleValue(vertex);

            // if (!point_inside_layer(x, y, sys->layerpoly)) {
            //     points->InsertNextPoint(x, y, z);
            //     hfield->InsertNextTupleValue(vertex);
            // }
        }
    }
    
    // We have to change these dimenstions if we want to 
    // exclude the fields inside the layers.
    structuredGrid->SetDimensions(sys->NGrid, sys->NGrid, 1);
    structuredGrid->SetPoints(points);
    structuredGrid->GetPointData()->SetScalars(hfield);

    vtkSmartPointer<vtkXMLStructuredGridWriter> writer = vtkSmartPointer<vtkXMLStructuredGridWriter>::New();
    std::string filename = sys->vtkpath + "hfield_struct_grid.vts";
    writer->SetFileName(filename.c_str());
    writer->SetInputData(structuredGrid);
    writer->Write();
}

void vtk_efield(System *sys, Grid *grid, std::map <std::string, Node *> filpoints) 
{
    vtkSmartPointer<vtkDoubleArray> efield = vtkSmartPointer<vtkDoubleArray>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkStructuredGrid> structuredGrid = vtkSmartPointer<vtkStructuredGrid>::New();

    efield->SetNumberOfComponents(3);
    efield->SetName("efield");
    
    for (auto const& fil : filpoints) {
        std::istringstream iss(fil.first);
        
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, 
                                        std::istream_iterator<std::string>{}};

        double r = fil.second->x;
        double g = fil.second->y;
        double b = fil.second->z;

        double vertex[3] = {r, g, b};

        double x = std::stod(tokens[0]) * 10e-12;
        double y = std::stod(tokens[1]) * 10e-12;
        double z = std::stod(tokens[2]) * 10e-12;

        points->InsertNextPoint(x, y, z);
        efield->InsertNextTupleValue(vertex);
    }

    vtkSmartPointer<vtkPolyData> pointsPolydata = vtkSmartPointer<vtkPolyData>::New();
    pointsPolydata->SetPoints(points);

    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vertexFilter->SetInputData(pointsPolydata);
    vertexFilter->Update();

    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->DeepCopy(vertexFilter->GetOutput());
    polydata->GetPointData()->SetVectors(efield);

    vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    std::string filename = sys->vtkpath + "efield.vtp";
    writer->SetFileName(filename.c_str());
    writer->SetInputData(polydata);
    writer->Write();
}

void vtk_hfield(System *sys, Grid *grid) 
{
    vtkSmartPointer<vtkDoubleArray> hfield = vtkSmartPointer<vtkDoubleArray>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    hfield->SetNumberOfComponents(3);
    hfield->SetName("hfield");

    for (int i = 0; i < sys->NGrid; i++) {
        for (int j = 0; j < sys->NGrid; j++) {
            double r = grid->Hx(i,j);
            double g = grid->Hy(i,j);
            double b = grid->Hz(i,j);

            double vertex[3] = {r, g, b};
            
            double x = grid->nx(i,j) * 10e-12;
            double y = grid->ny(i,j) * 10e-12;
            double z = grid->nz(i,j) * 10e-12;

            // if (!point_inside_layer(x, y, sys->layerpoly)) {
            //     points->InsertNextPoint(x, y, z);
            //     hfield->InsertNextTupleValue(vertex);
            // }

            hfield->InsertNextTupleValue(vertex);
            points->InsertNextPoint(x,y,z);
        }
    }

    vtkSmartPointer<vtkPolyData> pointsPolydata = vtkSmartPointer<vtkPolyData>::New();
    pointsPolydata->SetPoints(points);

    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vertexFilter->SetInputData(pointsPolydata);
    vertexFilter->Update();

    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->ShallowCopy(vertexFilter->GetOutput());
    polydata->GetPointData()->SetVectors(hfield);

    vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    std::string filename = sys->vtkpath + "hfield.vtp";    
    writer->SetFileName(filename.c_str());
    writer->SetInputData(polydata);
    writer->Write();
}













