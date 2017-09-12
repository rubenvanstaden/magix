#include <vtkSphereSource.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkImageData.h>
#include <vtkImageDataToPointSet.h>
#include <vtkXMLStructuredGridWriter.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkCellArray.h>
#include <vtkTextMapper.h>
#include <vtkCellData.h>
#include <vtkTriangle.h>
#include <vtkArrayCalculator.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkAppendFilter.h>
#include <vtkOctreePointLocator.h>
#include <vtkTransform.h>
#include <vtkRectilinearGrid.h>
#include <vtkSelectionNode.h>
#include <vtkImageSliceMapper.h>
#include "sys.h"
#include "grid.h"

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkPolygon.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkPoints.h>
#include <vtkXMLPolyDataWriter.h>

#include "clipper.hpp"

#include <vtkPoints.h>
#include <vtkPolygon.h>

#include <vtkSmartPointer.h>
#include <vtkVertexGlyphFilter.h>

#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>

#include <vtkNamedColors.h>

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkRegularPolygonSource.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkGlyph2D.h>
#include <vtkCellArray.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <vtkSphereSource.h>
#include <vtkArrowSource.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkGlyph3D.h>
#include <vtkCellArray.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkXMLPolyDataWriter.h>

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
#include <vtkPointSource.h>

#include <vtkPointData.h>
#include <vtkVertexGlyphFilter.h>

#include <vtkVersion.h>
#include <vtkSmartPointer.h>

#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkClipClosedSurface.h>
#include <vtkPlane.h>
#include <vtkPlaneCollection.h>
#include <vtkXMLPolyDataReader.h>

#include <vtkSphereSource.h>

#include <vtkActor.h>
#include <vtkIntersectionPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkSmoothPolyDataFilter.h>

#include <vtkVersion.h>
#include <vtkBooleanOperationPolyDataFilter.h>
 
#include <vtkActor.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

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
    create_grid(sys, grid, polygons, points);

    vtkSmartPointer<vtkPolyData> polygonPolyData = vtkSmartPointer<vtkPolyData>::New();
    polygonPolyData->SetPoints(points);
    polygonPolyData->SetPolys(polygons);

    vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    writer->SetFileName("magix_grid_polygons.vtp");
    writer->SetInputData(polygonPolyData);
    writer->Write();

    std::cout << "\n--- VTK grid finished polygons ---" << std::endl;    
}

void vtk_mag_field(System *sys, Grid *grid) 
{
    vtkSmartPointer<vtkFloatArray> b_field = vtkSmartPointer<vtkFloatArray>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkStructuredGrid> structuredGrid = vtkSmartPointer<vtkStructuredGrid>::New();

    float r, g, b;

    for (int i = 0; i < sys->NGrid; i++) {
        for (int j = 0; j < sys->NGrid; j++) {
            b_field->SetNumberOfComponents(3);
            b_field->SetName("b_field");

            r = (float) grid->Hx(i,j);
            g = (float) grid->Hy(i,j);
            b = (float) grid->Hz(i,j);

            // r = (float) image(i,j)(0);
            // g = (float) image(i,j)(1);
            // b = (float) image(i,j)(2);

            float vertex[3] = {r, g, b};

            double x = grid->nx(i,j);
            double y = grid->ny(i,j);
            double z = grid->nz(i,j);

            points->InsertNextPoint(x, y, z);
            b_field->InsertNextTupleValue(vertex);
        }
    }
    
    structuredGrid->SetDimensions(sys->NGrid, sys->NGrid, 1);
    structuredGrid->SetPoints(points);
    structuredGrid->GetPointData()->SetScalars(b_field);

    vtkSmartPointer<vtkXMLStructuredGridWriter> writer = vtkSmartPointer<vtkXMLStructuredGridWriter>::New();
    writer->SetFileName("magix_field.vts");
    writer->SetInputData(structuredGrid);
    writer->Write();
}

void vtk_current_sum(System *sys, Grid *grid, std::map <std::string, Node *> filpoints) 
{
    vtkSmartPointer<vtkDoubleArray> current_field = vtkSmartPointer<vtkDoubleArray>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkStructuredGrid> structuredGrid = vtkSmartPointer<vtkStructuredGrid>::New();

    current_field->SetNumberOfComponents(3);
    current_field->SetName("current_field");
    
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
        current_field->InsertNextTupleValue(vertex);
    }

    vtkSmartPointer<vtkPolyData> pointsPolydata = vtkSmartPointer<vtkPolyData>::New();
    pointsPolydata->SetPoints(points);

    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vertexFilter->SetInputData(pointsPolydata);
    vertexFilter->Update();

    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->DeepCopy(vertexFilter->GetOutput());
    polydata->GetPointData()->SetVectors(current_field);
    
    vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    
    writer->SetFileName("vtk_sum_current.vtp");
    writer->SetInputData(polydata);
    writer->Write();
}

void vtk_current(System *sys, Grid *grid) {
    vtkSmartPointer<vtkFloatArray> current_field = vtkSmartPointer<vtkFloatArray>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkStructuredGrid> structuredGrid = vtkSmartPointer<vtkStructuredGrid>::New();

    float r, g, b;

    current_field->SetNumberOfComponents(3);
    current_field->SetName("current_field");

    for (auto fil : sys->filVect) {
        r = (float) fil->stroom.x;
        g = (float) fil->stroom.y;
        b = (float) fil->stroom.z;

        float vertex[3] = {r, g, b};

        double x = fil->S.x * 10e-12;
        double y = fil->S.y * 10e-12;
        double z = fil->S.z * 10e-12;

        points->InsertNextPoint(x, y, z);
        current_field->InsertNextTupleValue(vertex);
    }

    vtkSmartPointer<vtkPolyData> pointsPolydata = vtkSmartPointer<vtkPolyData>::New();
    pointsPolydata->SetPoints(points);

    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vertexFilter->SetInputData(pointsPolydata);
    vertexFilter->Update();

    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->ShallowCopy(vertexFilter->GetOutput());

    polydata->GetPointData()->SetVectors(current_field);
    
    vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    
    writer->SetFileName("magix_current.vtp");
    writer->SetInputData(polydata);
    writer->Write();
}

void vtk_poly_points(System *sys, Grid *grid) {
    vtkSmartPointer<vtkFloatArray> b_field = vtkSmartPointer<vtkFloatArray>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkStructuredGrid> structuredGrid = vtkSmartPointer<vtkStructuredGrid>::New();

    float r, g, b;

    b_field->SetNumberOfComponents(3);
    b_field->SetName("b_field");

    for (int i = 0; i < sys->NGrid; i++) {
        for (int j = 0; j < sys->NGrid; j++) {
            r = (float) grid->Hx(i,j);
            g = (float) grid->Hy(i,j);
            b = (float) grid->Hz(i,j);

            float vertex[3] = {r, g, b};
            
            double x = grid->nx(i,j) * 10e-12;
            double y = grid->ny(i,j) * 10e-12;
            double z = grid->nz(i,j) * 10e-12;

            // double x = grid->nx(i,j);
            // double y = grid->ny(i,j);
            // double z = grid->nz(i,j);

            // unsigned char r[3] = {1,0,0};
            b_field->InsertNextTupleValue(vertex);
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
    polydata->GetPointData()->SetVectors(b_field);

    vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    
    writer->SetFileName("magix_points.vtp");
    writer->SetInputData(polydata);
    writer->Write();
}

void vtk_faces(System *sys, Grid *grid)
{
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polygons = vtkSmartPointer<vtkCellArray>::New();

    int count = 0;

    for (auto fil : sys->filVect) {
        vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
        polygon->GetPointIds()->SetNumberOfIds(4);

        for (int i = 0; i < 4; i++) {
            double x = fil->corners[i].y;
            double y = fil->corners[i].z;
            
            points->InsertNextPoint(x, y, 0.0);
            polygon->GetPointIds()->SetId(i, 4*count + i);
        }

        polygons->InsertNextCell(polygon);
        count++;
    }
    
    // Create a PolyData
    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(points);
    polydata->SetPolys(polygons);

    vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    
    writer->SetFileName("current_faces.vtp");
    writer->SetInputData(polydata);
    writer->Write();
}

void vtk_corners(System *sys, Grid *grid)
{
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polygons = vtkSmartPointer<vtkCellArray>::New();

    int count = 0;

    for (auto fil : sys->filVect) {
        for (int i = 0; i < 4; i++) {
            vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
            polygon->GetPointIds()->SetNumberOfIds(4);

            for (int j = 0; j < 4; j++) {
                double x = fil->poly[i][j].y;
                double y = fil->poly[i][j].z;

                points->InsertNextPoint(x, y, 0.0);
                polygon->GetPointIds()->SetId(j, 4*count + j);
            }

            polygons->InsertNextCell(polygon);   
            count++;            
        }
    }
    
    // Create a PolyData
    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(points);
    polydata->SetPolys(polygons);

    vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    
    writer->SetFileName("corners.vtp");
    writer->SetInputData(polydata);
    writer->Write();
}

void vtk_rect_currents(System *sys, Grid *grid)
{
    vtkSmartPointer<vtkFloatArray> b_field = vtkSmartPointer<vtkFloatArray>::New();    
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polygons = vtkSmartPointer<vtkCellArray>::New();

    int count = 0;
    
    b_field->SetNumberOfComponents(3);
    b_field->SetName("b_field");

    for (auto fil : sys->filVect) {
        vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
        polygon->GetPointIds()->SetNumberOfIds(4);

        for (int i = 0; i < 4; i++) {
            double x = fil->corners[i].y;
            double y = fil->corners[i].z;

            float r = (float) fil->stroom.x;
            float g = (float) fil->stroom.x;
            float b = (float) fil->stroom.x;
            
            float vertex[3] = {r, g, b};            
            
            b_field->InsertNextTupleValue(vertex);            
            points->InsertNextPoint(x, y, 0.0);
            polygon->GetPointIds()->SetId(i, 4*count + i);
        }

        polygons->InsertNextCell(polygon);
        count++;
    }

    vtkSmartPointer<vtkPolyData> pointsPolydata = vtkSmartPointer<vtkPolyData>::New();
    pointsPolydata->SetPoints(points);

    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vertexFilter->SetInputData(pointsPolydata);
    vertexFilter->Update();

    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->ShallowCopy(vertexFilter->GetOutput());
    polydata->GetPointData()->SetVectors(b_field);
    
    // // Create a PolyData
    // vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    // polydata->SetPoints(points);
    // polydata->SetPolys(polygons);

    vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    
    writer->SetFileName("current_faces.vtp");
    writer->SetInputData(polydata);
    writer->Write();
}

void vtk_x_filaments(System *sys, Grid *grid) {
    std::cout << "Start writing VTK file..." << std::endl;

    vtkSmartPointer<vtkUnstructuredGrid> uGridTwo = vtkSmartPointer<vtkUnstructuredGrid>::New();
    vtkSmartPointer<vtkPoints> pointsTwo = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> hexTwo = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkFloatArray> x_curr = vtkSmartPointer<vtkFloatArray>::New();
    
    Node *corners_segment = (Node *)calloc(8, sizeof(Node));

    for(int i = 0; i < sys->filVect.size(); i++) {
        x_curr->SetNumberOfComponents(3);
        x_curr->SetName("x_curr");

        for (int j = 0; j < 8; j++) {
            double px = sys->filVect[i]->corners[j].x;
            double py = sys->filVect[i]->corners[j].y;
            double pz = sys->filVect[i]->corners[j].z;
            
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

void vtk_bool_mapper(System *sys, Grid *grid)
{
    vtkSmartPointer<vtkPolyData> input1;
    vtkSmartPointer<vtkPolyData> input2;
   
    std::string operation("intersection");

    vtkSmartPointer<vtkSphereSource> sphereSource1 = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource1->SetCenter(.25, 0, 0);
    sphereSource1->Update();
    input1 = sphereSource1->GetOutput();

    vtkSmartPointer<vtkSphereSource> sphereSource2 = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource2->Update();
    input2 = sphereSource2->GetOutput();
    operation = "union";
   
    vtkSmartPointer<vtkPolyDataMapper> input1Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    input1Mapper->SetInputData( input1 );
    input1Mapper->ScalarVisibilityOff();
    vtkSmartPointer<vtkActor> input1Actor = vtkSmartPointer<vtkActor>::New();
    input1Actor->SetMapper( input1Mapper );
    input1Actor->GetProperty()->SetColor(1,0,0);
    input1Actor->SetPosition(input1->GetBounds()[1]-input1->GetBounds()[0], 0, 0);

    vtkSmartPointer<vtkPolyDataMapper> input2Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    input2Mapper->SetInputData( input2 );
    input2Mapper->ScalarVisibilityOff();
    vtkSmartPointer<vtkActor> input2Actor = vtkSmartPointer<vtkActor>::New();
    input2Actor->SetMapper( input2Mapper );
    input2Actor->GetProperty()->SetColor(0,1,0);
    input2Actor->SetPosition(-(input2->GetBounds()[1]-input2->GetBounds()[0]), 0, 0);
    
    vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
   
    if (operation == "union")
        booleanOperation->SetOperationToUnion();
    else if (operation == "intersection")
        booleanOperation->SetOperationToIntersection();
    else if (operation == "difference")
        booleanOperation->SetOperationToDifference();
    else
        std::cout << "Unknown operation: " << operation << std::endl;

    booleanOperation->SetInputData( 0, input1 );
    booleanOperation->SetInputData( 1, input2 );

    vtkSmartPointer<vtkPolyDataMapper> booleanOperationMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    booleanOperationMapper->SetInputConnection( booleanOperation->GetOutputPort() );
    booleanOperationMapper->ScalarVisibilityOff();

    // Start rendering
    vtkSmartPointer<vtkActor> booleanOperationActor = vtkSmartPointer<vtkActor>::New();
    booleanOperationActor->SetMapper( booleanOperationMapper );
 
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddViewProp(input1Actor);
    renderer->AddViewProp(input2Actor);
    renderer->AddViewProp(booleanOperationActor);
    renderer->SetBackground(.1, .2, .3);

    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer( renderer );

    vtkSmartPointer<vtkRenderWindowInteractor> renWinInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renWinInteractor->SetRenderWindow( renderWindow );
    renderWindow->Render();
    renWinInteractor->Start();

    // vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    // writer->SetFileName("awe.vtp");
    // writer->SetInputData(booleanOperationMapper->GetInput());
    // writer->Write();
}

void vtk_filament_intersections(System *sys, Grid *grid) 
{
    vtkSmartPointer<vtkPolyData> input1;
    vtkSmartPointer<vtkPolyData> input2;

    vtkSmartPointer<vtkSphereSource> sphereSource1 = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource1->SetCenter(0.0, 0.0, 0.0);
    sphereSource1->SetRadius(2.0f);
    sphereSource1->Update();
    // input1 = sphereSource1->GetOutputPort();

    vtkSmartPointer<vtkSphereSource> sphereSource2 = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource2->SetCenter(1.0, 0.0, 0.0);
    sphereSource2->SetRadius(2.0f);
    sphereSource2->Update();
    // input2 = sphereSource2->GetOutputPort();

    vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
    booleanOperation->SetOperationToIntersection();
    booleanOperation->SetInputConnection( 0, sphereSource1->GetOutputPort() );
    booleanOperation->SetInputConnection( 1, sphereSource2->GetOutputPort() );

    vtkSmartPointer<vtkPolyDataMapper> booleanOperationMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    booleanOperationMapper->SetInputConnection( booleanOperation->GetOutputPort() );
    booleanOperationMapper->ScalarVisibilityOff();

    vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    writer->SetFileName("bool.vtp");
    writer->SetInputData(booleanOperationMapper->GetInput());
    writer->Write();
}

void vtk_intersection(System *sys, Grid *grid)
{
    // vtkSmartPointer<vtkSphereSource> sphereSource = 
    // vtkSmartPointer<vtkSphereSource>::New();
    // sphereSource->SetRadius(10);
    // sphereSource->SetPhiResolution(50);
    // sphereSource->SetThetaResolution(50);
    // sphereSource->Update();

    // vtkSmartPointer<vtkPointSource> pointSource = 
    // vtkSmartPointer<vtkPointSource>::New();
    // pointSource->SetNumberOfPoints(40);
    // pointSource->SetRadius(2);
    // pointSource->Update();

    // {
    //     vtkSmartPointer<vtkXMLPolyDataWriter> writer = 
    //     vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    //     writer->SetFileName("input.vtp");
    //     writer->SetInputConnection(sphereSource->GetOutputPort());
    //     writer->Write();
    // }

    // {
    //     vtkSmartPointer<vtkXMLPolyDataWriter> writer = 
    //     vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    //     writer->SetFileName("points.vtp");
    //     writer->SetInputConnection(pointSource->GetOutputPort());
    //     writer->Write();
    // }

    // vtkSmartPointer<vtkIntersectionPolyDataFilter> smoothFilter = 
    // vtkSmartPointer<vtkIntersectionPolyDataFilter>::New();
    // smoothFilter->SetInputConnection(0, sphereSource->GetOutputPort());
    // smoothFilter->SetInputConnection(1, pointSource->GetOutputPort());
    // smoothFilter->Update();

    // vtkSmartPointer<vtkXMLPolyDataWriter> writer = 
    // vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    // writer->SetFileName("output.vtp");
    // writer->SetInputConnection(smoothFilter->GetOutputPort());
    // writer->Write();



    vtkSmartPointer<vtkSphereSource> sphereSource1 = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource1->SetCenter(0.0, 0.0, 0.0);
    sphereSource1->SetRadius(2.0f);
    sphereSource1->Update();

    // vtkSmartPointer<vtkPolyDataMapper> sphere1Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    // sphere1Mapper->SetInputConnection( sphereSource1->GetOutputPort() );
    // sphere1Mapper->ScalarVisibilityOff();
    
    // vtkSmartPointer<vtkActor> sphere1Actor = vtkSmartPointer<vtkActor>::New();
    // sphere1Actor->SetMapper( sphere1Mapper );
    // // sphere1Actor->GetProperty()->SetOpacity(.3);
    // sphere1Actor->GetProperty()->SetColor(1,0,0);
   
    vtkSmartPointer<vtkSphereSource> sphereSource2 = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource2->SetCenter(1.0, 0.0, 0.0);
    sphereSource2->SetRadius(2.0f);
    sphereSource2->Update();
    
    // vtkSmartPointer<vtkPolyDataMapper> sphere2Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    // sphere2Mapper->SetInputConnection( sphereSource2->GetOutputPort() );
    // sphere2Mapper->ScalarVisibilityOff();
    
    // vtkSmartPointer<vtkActor> sphere2Actor = vtkSmartPointer<vtkActor>::New();
    // sphere2Actor->SetMapper( sphere2Mapper );
    // // sphere2Actor->GetProperty()->SetOpacity(.3);
    // sphere2Actor->GetProperty()->SetColor(0,1,0);
   
    vtkSmartPointer<vtkIntersectionPolyDataFilter> intersectionPolyDataFilter = vtkSmartPointer<vtkIntersectionPolyDataFilter>::New();
    intersectionPolyDataFilter->SetInputConnection( 0, sphereSource1->GetOutputPort() );
    intersectionPolyDataFilter->SetInputConnection( 1, sphereSource2->GetOutputPort() );
    intersectionPolyDataFilter->Update();

    vtkSmartPointer<vtkSmoothPolyDataFilter> smoothFilter = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
    smoothFilter->SetInputConnection(0, sphereSource1->GetOutputPort());
    smoothFilter->SetInputConnection(1, sphereSource2->GetOutputPort());
    smoothFilter->Update();
   
    // vtkSmartPointer<vtkPolyDataMapper> intersectionMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    // intersectionMapper->SetInputConnection( intersectionPolyDataFilter->GetOutputPort() );
    // intersectionMapper->ScalarVisibilityOff();
   
    // vtkSmartPointer<vtkActor> intersectionActor = vtkSmartPointer<vtkActor>::New();
    // intersectionActor->SetMapper( intersectionMapper );

    // vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    // polydata->ShallowCopy(intersectionPolyDataFilter->GetOutput());

    vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    
    writer->SetFileName("intersection.vtp");
    writer->SetInputData(smoothFilter->GetOutput());
    writer->Write();
   
    // vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    // renderer->AddViewProp(sphere1Actor);
    // renderer->AddViewProp(sphere2Actor);
    // renderer->AddViewProp(intersectionActor);
   
    // vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    // renderWindow->AddRenderer( renderer );
   
    // vtkSmartPointer<vtkRenderWindowInteractor> renWinInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    // renWinInteractor->SetRenderWindow( renderWindow );
   
    // renderWindow->Render();
    // renWinInteractor->Start();
}

void vtk_clipper(System *sys, Grid *grid) 
{
    // PolyData to process
    vtkSmartPointer<vtkPolyData> polyData;

    vtkSmartPointer<vtkSphereSource> sphereSource =
    vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetThetaResolution(20);
    sphereSource->SetPhiResolution(11);
    sphereSource->Update();

    polyData = sphereSource->GetOutput();

    // if (argc > 1) {
    //     vtkSmartPointer<vtkXMLPolyDataReader> reader =
    //     vtkSmartPointer<vtkXMLPolyDataReader>::New();
    //     reader->SetFileName(argv[1]);
    //     reader->Update();
    //     polyData = reader->GetOutput();
    // }
    // else {
    //     // Create a sphere
    //     vtkSmartPointer<vtkSphereSource> sphereSource =
    //     vtkSmartPointer<vtkSphereSource>::New();
    //     sphereSource->SetThetaResolution(20);
    //     sphereSource->SetPhiResolution(11);
    //     sphereSource->Update();

    //     polyData = sphereSource->GetOutput();
    // }

    double *center = polyData->GetCenter();
   
    vtkSmartPointer<vtkPlane> plane1 = vtkSmartPointer<vtkPlane>::New();
    plane1->SetOrigin(center[0], center[1], center[2]);
    plane1->SetNormal(0.0, -1.0, 0.0);
   
    vtkSmartPointer<vtkPlane> plane2 = vtkSmartPointer<vtkPlane>::New();
    plane2->SetOrigin(center[0], center[1], center[2]);
    plane2->SetNormal(0.0, 0.0, 1.0);
   
    vtkSmartPointer<vtkPlane> plane3 = vtkSmartPointer<vtkPlane>::New();
    plane3->SetOrigin(center[0], center[1], center[2]);
    plane3->SetNormal(-1.0, 0.0, 0.0);

    vtkSmartPointer<vtkPlaneCollection> planes = vtkSmartPointer<vtkPlaneCollection>::New();
    planes->AddItem(plane1);
    planes->AddItem(plane2);
    planes->AddItem(plane3);

    vtkSmartPointer<vtkClipClosedSurface> clipper = vtkSmartPointer<vtkClipClosedSurface>::New();
    clipper->SetInputData(polyData);
    clipper->SetClippingPlanes(planes);
    clipper->SetActivePlaneId(2);
    // clipper->SetScalarModeToColors();
    // clipper->SetClipColor(0.8900, 0.8100, 0.3400); // banana
    // clipper->SetBaseColor(1.0000, 0.3882, 0.2784); // tomato
    // clipper->SetActivePlaneColor(0.6400, 0.5800, 0.5000); // beige

    vtkSmartPointer<vtkDataSetMapper> clipMapper =
    vtkSmartPointer<vtkDataSetMapper>::New();
    clipMapper->SetInputConnection(clipper->GetOutputPort());

    vtkSmartPointer<vtkActor> clipActor =
    vtkSmartPointer<vtkActor>::New();
    clipActor->SetMapper(clipMapper);
    clipActor->GetProperty()->SetColor(1.0000,0.3882,0.2784);
    clipActor->GetProperty()->SetInterpolationToFlat();

    // Create graphics stuff
    vtkSmartPointer<vtkRenderer> ren1 =
    vtkSmartPointer<vtkRenderer>::New();
    ren1->SetBackground(.3, .4, .6);

    vtkSmartPointer<vtkRenderWindow> renWin =
    vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(ren1);
    renWin->SetSize(512,512);

    vtkSmartPointer<vtkRenderWindowInteractor> iren =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
    iren->SetRenderWindow(renWin);

    ren1->AddActor(clipActor);

    ren1->ResetCamera();
    ren1->GetActiveCamera()->Azimuth(120);
    ren1->GetActiveCamera()->Elevation(30);
    ren1->GetActiveCamera()->Dolly(1.0);
    ren1->ResetCameraClippingRange();

    iren->Initialize();
    iren->Start();
}














