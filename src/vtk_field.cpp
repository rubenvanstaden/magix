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
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkAppendFilter.h>
#include <vtkOctreePointLocator.h>
#include <vtkTransform.h>
#include <vtkRectilinearGrid.h>
#include <vtkSelectionNode.h>
#include <vtkImageSliceMapper.h>
#include "sys.h"

#include <armadillo>

void vtk_mag_field(System *sys, arma::field<arma::rowvec> image, arma::field<arma::rowvec> obs_grid) {

    vtkSmartPointer<vtkFloatArray> b_field = vtkSmartPointer<vtkFloatArray>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkStructuredGrid> structuredGrid = vtkSmartPointer<vtkStructuredGrid>::New();

    unsigned int numi = sys->NGrid;
    unsigned int numj = sys->NGrid;
    unsigned int numk = 1;

    float r, g, b;
    double x, y, z;

    for (unsigned int k = 0; k < numk; k++) {
        for (unsigned int j = 2*sys->grid_width; j < sys->NGrid - 2*sys->grid_width; j++) {
            for (unsigned int i = 2*sys->grid_width; i < sys->NGrid - 2*sys->grid_width; i++) {
                b_field->SetNumberOfComponents(3);
                b_field->SetName("b_field");

                if (sys->glyph_view == 1) {
                    r = (float) sys->BField[j][i].x;
                    g = (float) sys->BField[j][i].z;
                    b = (float) sys->BField[j][i].y;
                } else {
                    r = (float) image(i,j)(0);
                    g = (float) image(i,j)(1);
                    b = (float) image(i,j)(2);
                }

                float vertex[3] = {r, g, b};
                b_field->InsertNextTupleValue(vertex);

                x = obs_grid(0)(0);
                y = obs_grid(j)(1);
                z = obs_grid(i)(2);

                // if (sys->top_view == 0) {
                //     x = obs_grid(0)(0);
                //     y = obs_grid(j)(1);
                //     z = obs_grid(i)(2);

                //     // if (sys->glyph_view == 1) {
                //     //     x = sys->obs_grid[0].x;
                //     //     y = sys->obs_grid[i].y;
                //     //     z = sys->obs_grid[j].z;
                //     // } else {
                //     //     x = sys->obs_grid[0].x;
                //     //     y = sys->obs_grid[j].y;
                //     //     z = sys->obs_grid[i].z;
                //     // }
                // } else {
                //     z = obs_grid(0)(0);
                //     y = obs_grid(j)(1);
                //     x = obs_grid(i)(2);

                //     // z = sys->obs_grid[0].x;
                //     // x = sys->obs_grid[j].y;
                //     // y = sys->obs_grid[i].z;
                // }

                points->InsertNextPoint(x, y, z);
            }
        }
    }

    printf("Finished!");

    structuredGrid->SetDimensions(numi - 4*sys->grid_width, numj - 4*sys->grid_width, numk);
    structuredGrid->SetPoints(points);
    structuredGrid->GetPointData()->SetVectors(b_field);

    vtkSmartPointer<vtkXMLStructuredGridWriter> writer = vtkSmartPointer<vtkXMLStructuredGridWriter>::New();
    writer->SetFileName("magix_vtk.vts");
    writer->SetInputData(structuredGrid);
    writer->Write();
}







































