//--------------------------------------------------
// BLDC Simulation
// projectScript.cpp
// Date: 2023-09-27
//--------------------------------------------------
#include "projectScript.h"
#include "imgui.h"
#include "implot.h"

void ProjectScript::onStart() {}
void ProjectScript::onStop() {}

void ProjectScript::onUpdateBefore(float dt) { LOG_DEBUG("Project", "Update $0", dt); }

void ProjectScript::onUIRender() {
    // Setup static data
    static float x_data[100];
    static float y_data[100];
    for (int i = 0; i < 100; i++) {
        x_data[i] = i;
        y_data[i] = sin(i * 0.1f);
    }

    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Once);
    ImGui::Begin("Plots");
    {
        ImGui::Text("Input");

        if (ImPlot::BeginPlot("Phase voltages")) {
            ImPlot::PlotLine("v_u", x_data, y_data, 100);
            ImPlot::PlotLine("v_v", x_data, y_data, 100);
            ImPlot::PlotLine("v_w", x_data, y_data, 100);
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Load torque")) {
            ImPlot::PlotLine("T_l", x_data, y_data, 100);
            ImPlot::EndPlot();
        }

        ImGui::Text("State");
        if (ImPlot::BeginPlot("Currents")) {
            ImPlot::PlotLine("i_u", x_data, y_data, 100);
            ImPlot::PlotLine("i_v", x_data, y_data, 100);
            ImPlot::PlotLine("i_w", x_data, y_data, 100);
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Motor speed")) {
            ImPlot::PlotLine("w_m", x_data, y_data, 100);
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Rotor position")) {
            ImPlot::PlotLine("θ_r", x_data, y_data, 100);
            ImPlot::EndPlot();
        }

        ImGui::Text("Derived");
        if (ImPlot::BeginPlot("Back EMF")) {
            ImPlot::PlotLine("r_u", x_data, y_data, 100);
            ImPlot::PlotLine("r_v", x_data, y_data, 100);
            ImPlot::PlotLine("r_w", x_data, y_data, 100);
            ImPlot::EndPlot();
        }
    }
    ImGui::End();
}
