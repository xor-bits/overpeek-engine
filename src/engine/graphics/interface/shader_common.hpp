#pragma once

#include <shaderc/shaderc.hpp>
#include "engine/utility/fileio.hpp"
#include "engine/internal_libs.hpp"



namespace oe::graphics
{
    shaderc_shader_kind shader_kind(oe::shader_stages stage) {
        switch (stage)
        {
        case oe::shader_stages::vertex_shader:
            return shaderc_shader_kind::shaderc_vertex_shader;
            break;
        case oe::shader_stages::tesselation_control_shader:
            return shaderc_shader_kind::shaderc_tess_control_shader;
            break;
        case oe::shader_stages::tesselation_evaluation_shader:
            return shaderc_shader_kind::shaderc_tess_evaluation_shader;
            break;
        case oe::shader_stages::geometry_shader:
            return shaderc_shader_kind::shaderc_geometry_shader;
            break;
        case oe::shader_stages::fragment_shader:
            return shaderc_shader_kind::shaderc_fragment_shader;
            break;
        case oe::shader_stages::compute_shader:
            return shaderc_shader_kind::shaderc_compute_shader;
            break;
        }
    }

    std::string shader_module_name(const std::string& name, oe::shader_stages stage) {
        switch (stage)
        {
        case oe::shader_stages::vertex_shader:
            return name + "." + "vertex";
            break;
        case oe::shader_stages::tesselation_control_shader:
            return name + "." + "tess_control";
            break;
        case oe::shader_stages::tesselation_evaluation_shader:
            return name + "." + "tess_evaluation";
            break;
        case oe::shader_stages::geometry_shader:
            return name + "." + "geometry";
            break;
        case oe::shader_stages::fragment_shader:
            return name + "." + "fragment";
            break;
        case oe::shader_stages::compute_shader:
            return name + "." + "compute";
            break;
        }
    }

    struct include_result : public shaderc_include_result
    {
        std::string content_cpp_string;
        std::string source_name_cpp_string;
    };
    
    class Includer : public shaderc::CompileOptions::IncluderInterface {
        const fs::path relative;

    public:
        Includer(fs::path _relative)
            : relative(_relative)
        {}

        // Handles shaderc_include_resolver_fn callbacks.
        virtual shaderc_include_result* GetInclude(const char* requested_source,
                                                shaderc_include_type type,
                                                const char* requesting_source,
                                                size_t include_depth) override
        {
            auto data = new include_result();
            oe::utils::FileIO::getSingleton().readString(data->content_cpp_string, relative / requested_source);
            data->source_name_cpp_string = fs::path(requested_source).filename().string();
            data->content = data->content_cpp_string.c_str();
            data->content_length = data->content_cpp_string.size();

            data->source_name = data->source_name_cpp_string.c_str();
            data->source_name_length = data->source_name_cpp_string.size();

            return data;
        }

        // Handles shaderc_include_result_release_fn callbacks.
        virtual void ReleaseInclude(shaderc_include_result* data) override
        {
            delete data;
        }
    };
}