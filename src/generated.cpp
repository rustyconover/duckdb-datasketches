#include "datasketches_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/common/extra_type_info.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/main/extension_util.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>
#include <duckdb/parser/parsed_data/create_aggregate_function_info.hpp>

#include <DataSketches/quantiles_sketch.hpp>
#include <DataSketches/kll_sketch.hpp>
#include <DataSketches/req_sketch.hpp>
#include <DataSketches/tdigest.hpp>
#include <DataSketches/hll.hpp>
#include <DataSketches/cpc_sketch.hpp>
#include <DataSketches/cpc_union.hpp>
#include <optional>

namespace duckdb
{

static std::string toLowerCase(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return result;
}

    

    
    template <class T>
    
    struct DSQuantilesState
    {
        
        datasketches::quantiles_sketch<T> *sketch = nullptr;
        

        ~DSQuantilesState()
        {
            if (sketch)
            {
                delete sketch;
            }
        }

        
        void CreateSketch(int32_t k)
        {
            D_ASSERT(!sketch);
            D_ASSERT(k > 0);
            D_ASSERT(k <= 32768);
            sketch = new datasketches::quantiles_sketch<T>(k);
        }
        

        void CreateSketch(const DSQuantilesState &existing)
        {
            if (existing.sketch)
            {
                
                sketch = new datasketches::quantiles_sketch<T>(*existing.sketch);
                
            }
        }

        
        datasketches::quantiles_sketch<T> deserialize_sketch(const string_t &data)
        {
            return datasketches::quantiles_sketch<T>::deserialize(data.GetDataUnsafe(), data.GetSize());
        }
        
    };



    
    static LogicalType CreateQuantilesSketchType(DatabaseInstance &instance, LogicalType embedded_type)
    {
        LogicalType new_type = LogicalType::BLOB;
        auto type_suffix = toLowerCase(embedded_type.ToString());
        auto new_type_name = "sketch_quantiles_" + type_suffix;
        auto type_info = CreateTypeInfo(new_type_name, LogicalType::BLOB);
        type_info.temporary = false;
        type_info.internal = true;
        new_type.SetAlias(new_type_name);
        auto &system_catalog = Catalog::GetSystemCatalog(instance);
        auto data = CatalogTransaction::GetSystemTransaction(instance);
        system_catalog.CreateType(data, type_info);
	    ExtensionUtil::RegisterCastFunction(instance, LogicalType::BLOB, new_type, DefaultCasts::ReinterpretCast, 1);
        ExtensionUtil::RegisterCastFunction(instance, new_type, LogicalType::BLOB, DefaultCasts::ReinterpretCast, 1);

        return new_type;
    }
    

    
    template <class T>
    
    struct DSKLLState
    {
        
        datasketches::kll_sketch<T> *sketch = nullptr;
        

        ~DSKLLState()
        {
            if (sketch)
            {
                delete sketch;
            }
        }

        
        void CreateSketch(int32_t k)
        {
            D_ASSERT(!sketch);
            D_ASSERT(k > 0);
            D_ASSERT(k <= 32768);
            sketch = new datasketches::kll_sketch<T>(k);
        }
        

        void CreateSketch(const DSKLLState &existing)
        {
            if (existing.sketch)
            {
                
                sketch = new datasketches::kll_sketch<T>(*existing.sketch);
                
            }
        }

        
        datasketches::kll_sketch<T> deserialize_sketch(const string_t &data)
        {
            return datasketches::kll_sketch<T>::deserialize(data.GetDataUnsafe(), data.GetSize());
        }
        
    };



    
    static LogicalType CreateKLLSketchType(DatabaseInstance &instance, LogicalType embedded_type)
    {
        LogicalType new_type = LogicalType::BLOB;
        auto type_suffix = toLowerCase(embedded_type.ToString());
        auto new_type_name = "sketch_kll_" + type_suffix;
        auto type_info = CreateTypeInfo(new_type_name, LogicalType::BLOB);
        type_info.temporary = false;
        type_info.internal = true;
        new_type.SetAlias(new_type_name);
        auto &system_catalog = Catalog::GetSystemCatalog(instance);
        auto data = CatalogTransaction::GetSystemTransaction(instance);
        system_catalog.CreateType(data, type_info);
	    ExtensionUtil::RegisterCastFunction(instance, LogicalType::BLOB, new_type, DefaultCasts::ReinterpretCast, 1);
        ExtensionUtil::RegisterCastFunction(instance, new_type, LogicalType::BLOB, DefaultCasts::ReinterpretCast, 1);

        return new_type;
    }
    

    
    template <class T>
    
    struct DSREQState
    {
        
        datasketches::req_sketch<T> *sketch = nullptr;
        

        ~DSREQState()
        {
            if (sketch)
            {
                delete sketch;
            }
        }

        
        void CreateSketch(int32_t k)
        {
            D_ASSERT(!sketch);
            D_ASSERT(k >= 4);
            D_ASSERT(k <= 1024);
            sketch = new datasketches::req_sketch<T>(k);
        }
        

        void CreateSketch(const DSREQState &existing)
        {
            if (existing.sketch)
            {
                
                sketch = new datasketches::req_sketch<T>(*existing.sketch);
                
            }
        }

        
        datasketches::req_sketch<T> deserialize_sketch(const string_t &data)
        {
            return datasketches::req_sketch<T>::deserialize(data.GetDataUnsafe(), data.GetSize());
        }
        
    };



    
    static LogicalType CreateREQSketchType(DatabaseInstance &instance, LogicalType embedded_type)
    {
        LogicalType new_type = LogicalType::BLOB;
        auto type_suffix = toLowerCase(embedded_type.ToString());
        auto new_type_name = "sketch_req_" + type_suffix;
        auto type_info = CreateTypeInfo(new_type_name, LogicalType::BLOB);
        type_info.temporary = false;
        type_info.internal = true;
        new_type.SetAlias(new_type_name);
        auto &system_catalog = Catalog::GetSystemCatalog(instance);
        auto data = CatalogTransaction::GetSystemTransaction(instance);
        system_catalog.CreateType(data, type_info);
	    ExtensionUtil::RegisterCastFunction(instance, LogicalType::BLOB, new_type, DefaultCasts::ReinterpretCast, 1);
        ExtensionUtil::RegisterCastFunction(instance, new_type, LogicalType::BLOB, DefaultCasts::ReinterpretCast, 1);

        return new_type;
    }
    

    
    template <class T>
    
    struct DSTDigestState
    {
        
        datasketches::tdigest<T> *sketch = nullptr;
        

        ~DSTDigestState()
        {
            if (sketch)
            {
                delete sketch;
            }
        }

        
        void CreateSketch(uint16_t k)
        {
            D_ASSERT(!sketch);
            sketch = new datasketches::tdigest<T>(k);
        }
        

        void CreateSketch(const DSTDigestState &existing)
        {
            if (existing.sketch)
            {
                
                sketch = new datasketches::tdigest<T>(*existing.sketch);
                
            }
        }

        
        datasketches::tdigest<T> deserialize_sketch(const string_t &data)
        {
            return datasketches::tdigest<T>::deserialize(data.GetDataUnsafe(), data.GetSize());
        }
        
    };



    
    static LogicalType CreateTDigestSketchType(DatabaseInstance &instance, LogicalType embedded_type)
    {
        LogicalType new_type = LogicalType::BLOB;
        auto type_suffix = toLowerCase(embedded_type.ToString());
        auto new_type_name = "sketch_tdigest_" + type_suffix;
        auto type_info = CreateTypeInfo(new_type_name, LogicalType::BLOB);
        type_info.temporary = false;
        type_info.internal = true;
        new_type.SetAlias(new_type_name);
        auto &system_catalog = Catalog::GetSystemCatalog(instance);
        auto data = CatalogTransaction::GetSystemTransaction(instance);
        system_catalog.CreateType(data, type_info);
	    ExtensionUtil::RegisterCastFunction(instance, LogicalType::BLOB, new_type, DefaultCasts::ReinterpretCast, 1);
        ExtensionUtil::RegisterCastFunction(instance, new_type, LogicalType::BLOB, DefaultCasts::ReinterpretCast, 1);

        return new_type;
    }
    

    
    struct DSHLLState
    {
        
        datasketches::hll_sketch *sketch = nullptr;
        

        ~DSHLLState()
        {
            if (sketch)
            {
                delete sketch;
            }
        }

        
        void CreateSketch(uint16_t k)
        {
            D_ASSERT(!sketch);
            sketch = new datasketches::hll_sketch(k);
        }
        

        void CreateSketch(const DSHLLState &existing)
        {
            if (existing.sketch)
            {
                
                sketch = new datasketches::hll_sketch(*existing.sketch);
                
            }
        }

        
        datasketches::hll_sketch deserialize_sketch(const string_t &data)
        {
            return datasketches::hll_sketch::deserialize(data.GetDataUnsafe(), data.GetSize());
        }
        
    };



    
    static LogicalType CreateHLLCountingSketchType(DatabaseInstance &instance)
    {
        LogicalType new_type = LogicalType::BLOB;
        auto new_type_name = "sketch_hll";
        auto type_info = CreateTypeInfo(new_type_name, LogicalType::BLOB);
        type_info.temporary = false;
        type_info.internal = true;
        new_type.SetAlias(new_type_name);
        auto &system_catalog = Catalog::GetSystemCatalog(instance);
        auto data = CatalogTransaction::GetSystemTransaction(instance);
        system_catalog.CreateType(data, type_info);
	    ExtensionUtil::RegisterCastFunction(instance, LogicalType::BLOB, new_type, DefaultCasts::ReinterpretCast, 2);
        ExtensionUtil::RegisterCastFunction(instance, new_type, LogicalType::BLOB, DefaultCasts::ReinterpretCast, 2);
        return new_type;
    }
    

    
    struct DSCPCState
    {
        
        datasketches::cpc_sketch *sketch = nullptr;
        

        ~DSCPCState()
        {
            if (sketch)
            {
                delete sketch;
            }
        }

        
        void CreateSketch(uint8_t k)
        {
            D_ASSERT(!sketch);
            sketch = new datasketches::cpc_sketch(k);
        }
        

        void CreateSketch(const DSCPCState &existing)
        {
            if (existing.sketch)
            {
                
                sketch = new datasketches::cpc_sketch(*existing.sketch);
                
            }
        }

        
        datasketches::cpc_sketch deserialize_sketch(const string_t &data)
        {
            return datasketches::cpc_sketch::deserialize(data.GetDataUnsafe(), data.GetSize());
        }
        
    };



    
    static LogicalType CreateCPCCountingSketchType(DatabaseInstance &instance)
    {
        LogicalType new_type = LogicalType::BLOB;
        auto new_type_name = "sketch_cpc";
        auto type_info = CreateTypeInfo(new_type_name, LogicalType::BLOB);
        type_info.temporary = false;
        type_info.internal = true;
        new_type.SetAlias(new_type_name);
        auto &system_catalog = Catalog::GetSystemCatalog(instance);
        auto data = CatalogTransaction::GetSystemTransaction(instance);
        system_catalog.CreateType(data, type_info);
	    ExtensionUtil::RegisterCastFunction(instance, LogicalType::BLOB, new_type, DefaultCasts::ReinterpretCast, 2);
        ExtensionUtil::RegisterCastFunction(instance, new_type, LogicalType::BLOB, DefaultCasts::ReinterpretCast, 2);
        return new_type;
    }
    

    struct DSQuantilesMergeOperation
    {
        template <class STATE>
        static void Initialize(STATE &state)
        {
            state.sketch = nullptr;
        }

        template <class A_TYPE, class B_TYPE, class STATE, class OP>
        static void Operation(STATE &state,
                              const A_TYPE &a_data,
                              const B_TYPE &b_data,
                              AggregateBinaryInput &idata)
        {
            if (!state.sketch)
            {
                state.CreateSketch(a_data);
            }

            // this is a sketch in b_data, so we need to deserialize it.
            state.sketch->merge(state.deserialize_sketch(b_data));
        }

        template <class STATE, class OP>
        static void Combine(const STATE &source, STATE &target,
                            AggregateInputData &aggr_input_data)
        {
            if (!target.sketch)
            {
                target.CreateSketch(source);
            }
            else
            {
                target.sketch->merge(*source.sketch);
            }
        }

        template <class T, class STATE>
        static void Finalize(STATE &state, T &target,
                             AggregateFinalizeData &finalize_data)
        {
            if (!state.sketch)
            {
                finalize_data.ReturnNull();
            }
            else
            {
                auto serialized_data = state.sketch->serialize();
                auto sketch_string = std::string(serialized_data.begin(), serialized_data.end());
                delete state.sketch;
                state.sketch = nullptr;

                target = StringVector::AddStringOrBlob(finalize_data.result, sketch_string);
            }
        }

        static bool IgnoreNull() { return true; }
    };

    struct DSQuantilesCreateOperation
    {
        template <class STATE>
        static void Initialize(STATE &state)
        {
            state.sketch = nullptr;
        }

        template <class A_TYPE, class B_TYPE, class STATE, class OP>
        static void Operation(STATE &state,
                              const A_TYPE &a_data,
                              const B_TYPE &b_data,
                              AggregateBinaryInput &idata)
        {
            if (!state.sketch)
            {
                state.CreateSketch(a_data);
            }

            state.sketch->update(b_data);
        }

        template <class STATE, class OP>
        static void Combine(const STATE &source, STATE &target,
                            AggregateInputData &aggr_input_data)
        {
            if (!target.sketch)
            {
                target.CreateSketch(source);
            }
            else
            {
                target.sketch->merge(*source.sketch);
            }
        }

        template <class T, class STATE>
        static void Finalize(STATE &state, T &target,
                             AggregateFinalizeData &finalize_data)
        {
            if (!state.sketch)
            {
                finalize_data.ReturnNull();
            }
            else
            {
                auto serialized_data = state.sketch->serialize();
                auto sketch_string = std::string(serialized_data.begin(), serialized_data.end());
                delete state.sketch;
                state.sketch = nullptr;

                target = StringVector::AddStringOrBlob(finalize_data.result, sketch_string);
            }
        }

        static bool IgnoreNull() { return true; }
    };


    struct DSHLLCreateOperation
    {
        template <class STATE>
        static void Initialize(STATE &state)
        {
            state.sketch = nullptr;
        }

        template <class A_TYPE, class B_TYPE, class STATE, class OP>
        static void Operation(STATE &state,
                              const A_TYPE &a_data,
                              const B_TYPE &b_data,
                              AggregateBinaryInput &idata)
        {
            if (!state.sketch)
            {
                state.CreateSketch(a_data);
            }

            if constexpr (std::is_same_v<B_TYPE, duckdb::string_t>) {
                state.sketch->update(b_data.GetData(), b_data.GetSize());
            } else {
                state.sketch->update(b_data);
            }
        }

        template <class STATE, class OP>
        static void Combine(const STATE &source, STATE &target,
                            AggregateInputData &aggr_input_data)
        {
            if (!target.sketch)
            {
                target.CreateSketch(source);
            }
            else
            {
                datasketches::hll_union u(target.sketch->get_lg_config_k());
                u.update(*target.sketch);
                u.update(*source.sketch);

                *target.sketch = u.get_result(datasketches::target_hll_type::HLL_4);
            }
        }

        template <class T, class STATE>
        static void Finalize(STATE &state, T &target,
                             AggregateFinalizeData &finalize_data)
        {
            if (!state.sketch)
            {
                finalize_data.ReturnNull();
            }
            else
            {
                auto serialized_data = state.sketch->serialize_updatable();
                auto sketch_string = std::string(serialized_data.begin(), serialized_data.end());
                delete state.sketch;
                state.sketch = nullptr;

                target = StringVector::AddStringOrBlob(finalize_data.result, sketch_string);
            }
        }

        static bool IgnoreNull() { return true; }
    };


    struct DSHLLMergeOperation
    {
        template <class STATE>
        static void Initialize(STATE &state)
        {
            state.sketch = nullptr;
        }

        template <class A_TYPE, class B_TYPE, class STATE, class OP>
        static void Operation(STATE &state,
                              const A_TYPE &a_data,
                              const B_TYPE &b_data,
                              AggregateBinaryInput &idata)
        {
            if (!state.sketch)
            {
                state.CreateSketch(a_data);
            }

            auto b_sketch = state.deserialize_sketch(b_data);

            datasketches::hll_union u(a_data);
            u.update(*state.sketch);
            u.update(b_sketch);

            *state.sketch = u.get_result(datasketches::target_hll_type::HLL_4);
        }

        template <class STATE, class OP>
        static void Combine(const STATE &source, STATE &target,
                            AggregateInputData &aggr_input_data)
        {
            if (!target.sketch)
            {
                target.CreateSketch(source);
            }
            else
            {
                datasketches::hll_union u(target.sketch->get_lg_config_k());

                u.update(*source.sketch);
                u.update(*target.sketch);

                *target.sketch = u.get_result(datasketches::target_hll_type::HLL_4);
            }
        }

        template <class T, class STATE>
        static void Finalize(STATE &state, T &target,
                             AggregateFinalizeData &finalize_data)
        {
            if (!state.sketch)
            {
                finalize_data.ReturnNull();
            }
            else
            {
                auto serialized_data = state.sketch->serialize_updatable();
                auto sketch_string = std::string(serialized_data.begin(), serialized_data.end());
                delete state.sketch;
                state.sketch = nullptr;
                target = StringVector::AddStringOrBlob(finalize_data.result, sketch_string);
            }
        }

        static bool IgnoreNull() { return true; }
    };


    struct DSCPCMergeOperation
    {
        template <class STATE>
        static void Initialize(STATE &state)
        {
            state.sketch = nullptr;
        }

        template <class A_TYPE, class B_TYPE, class STATE, class OP>
        static void Operation(STATE &state,
                              const A_TYPE &a_data,
                              const B_TYPE &b_data,
                              AggregateBinaryInput &idata)
        {
            if (!state.sketch)
            {
                state.CreateSketch(a_data);
            }

            auto b_sketch = state.deserialize_sketch(b_data);

            datasketches::cpc_union u(a_data);
            u.update(*state.sketch);
            u.update(b_sketch);

            *state.sketch = u.get_result();
        }

        template <class STATE, class OP>
        static void Combine(const STATE &source, STATE &target,
                            AggregateInputData &aggr_input_data)
        {
            if (!target.sketch)
            {
                target.CreateSketch(source);
            }
            else
            {
                datasketches::cpc_union u(target.sketch->get_lg_k());
                u.update(*source.sketch);
                u.update(*target.sketch);
                *target.sketch = u.get_result();
            }
        }

        template <class T, class STATE>
        static void Finalize(STATE &state, T &target,
                             AggregateFinalizeData &finalize_data)
        {
            if (!state.sketch)
            {
                finalize_data.ReturnNull();
            }
            else
            {
                auto serialized_data = state.sketch->serialize();
                auto sketch_string = std::string(serialized_data.begin(), serialized_data.end());
                delete state.sketch;
                state.sketch = nullptr;
                target = StringVector::AddStringOrBlob(finalize_data.result, sketch_string);
            }
        }

        static bool IgnoreNull() { return true; }
    };

    struct DSCPCCreateOperation
    {
        template <class STATE>
        static void Initialize(STATE &state)
        {
            state.sketch = nullptr;
        }

        template <class A_TYPE, class B_TYPE, class STATE, class OP>
        static void Operation(STATE &state,
                              const A_TYPE &a_data,
                              const B_TYPE &b_data,
                              AggregateBinaryInput &idata)
        {
            if (!state.sketch)
            {
                state.CreateSketch(a_data);
            }

            if constexpr (std::is_same_v<B_TYPE, duckdb::string_t>) {
                state.sketch->update(b_data.GetData(), b_data.GetSize());
            } else {
                state.sketch->update(b_data);
            }
        }

        template <class STATE, class OP>
        static void Combine(const STATE &source, STATE &target,
                            AggregateInputData &aggr_input_data)
        {
            if (!target.sketch)
            {
                target.CreateSketch(source);
            }
            else
            {
                datasketches::cpc_union u(target.sketch->get_lg_k());
                u.update(*target.sketch);
                u.update(*source.sketch);
                *target.sketch = u.get_result();
            }
        }

        template <class T, class STATE>
        static void Finalize(STATE &state, T &target,
                             AggregateFinalizeData &finalize_data)
        {
            if (!state.sketch)
            {
                finalize_data.ReturnNull();
            }
            else
            {
                auto serialized_data = state.sketch->serialize();
                auto sketch_string = std::string(serialized_data.begin(), serialized_data.end());
                delete state.sketch;
                state.sketch = nullptr;

                target = StringVector::AddStringOrBlob(finalize_data.result, sketch_string);
            }
        }

        static bool IgnoreNull() { return true; }
    };




    


template <class T>

static inline void DSQuantilesis_empty(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,bool>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::quantiles_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.is_empty();
        });

}

    


template <class T>

static inline void DSQuantilesk(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,uint16_t>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::quantiles_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_k();
        });

}

    


template <class T>

static inline void DSQuantilescdf(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 3);

    
        auto &sketch_vector = args.data[0];
        auto &split_points_vector = args.data[1];
        auto &inclusive_vector = args.data[2];

    
        
                    UnifiedVectorFormat unified_split_points;
                    split_points_vector.ToUnifiedFormat(args.size(), unified_split_points);

                    // auto split_points_list_entries = UnifiedVectorFormat::GetData<list_entry_t>(unified_split_points);
                    //         auto split_points_validitiy = FlatVector::Validity(split_points_vector);

                    auto &split_points_list_children = ListVector::GetEntry(split_points_vector);

                    UnifiedVectorFormat split_points_children_unified;
                    split_points_list_children.ToUnifiedFormat(args.size(), split_points_children_unified);

                    const T *split_points_list_children_data = UnifiedVectorFormat::GetData<T>(split_points_children_unified);
                    
        TernaryExecutor::Execute
        <string_t,list_entry_t,bool,list_entry_t>
        (
        sketch_vector,split_points_vector,inclusive_vector,result,args.size(),
        [&](string_t sketch_data,list_entry_t split_points_data,bool inclusive_data) {

            auto sketch = datasketches::quantiles_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());

                    T *passing_points = (T *)duckdb_malloc(sizeof(T) * split_points_data.length);
                    for (idx_t i = 0; i < split_points_data.length; i++)
                    {
                        passing_points[i] = split_points_list_children_data[i + split_points_data.offset];
                    }
                    
auto cdf_result = sketch.get_CDF(passing_points, split_points_data.length, inclusive_data);
                auto current_size = ListVector::GetListSize(result);
                auto new_size = current_size + cdf_result.size();
                if (ListVector::GetListCapacity(result) < new_size)
                {
                    ListVector::Reserve(result, new_size);
                }

                auto &child_entry = ListVector::GetEntry(result);
                auto child_vals = FlatVector::GetData<T>(child_entry);
                //auto &child_validity = FlatVector::Validity(child_entry);
                for (idx_t i = 0; i < cdf_result.size(); i++)
                {
                    child_vals[current_size + i] = cdf_result[i];
                }
                ListVector::SetListSize(result, new_size);
                return list_entry_t{current_size, cdf_result.size()};
                
        });

}

    


template <class T>

static inline void DSQuantilespmf(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 3);

    
        auto &sketch_vector = args.data[0];
        auto &split_points_vector = args.data[1];
        auto &inclusive_vector = args.data[2];

    
        
                    UnifiedVectorFormat unified_split_points;
                    split_points_vector.ToUnifiedFormat(args.size(), unified_split_points);

                    // auto split_points_list_entries = UnifiedVectorFormat::GetData<list_entry_t>(unified_split_points);
                    //         auto split_points_validitiy = FlatVector::Validity(split_points_vector);

                    auto &split_points_list_children = ListVector::GetEntry(split_points_vector);

                    UnifiedVectorFormat split_points_children_unified;
                    split_points_list_children.ToUnifiedFormat(args.size(), split_points_children_unified);

                    const T *split_points_list_children_data = UnifiedVectorFormat::GetData<T>(split_points_children_unified);
                    
        TernaryExecutor::Execute
        <string_t,list_entry_t,bool,list_entry_t>
        (
        sketch_vector,split_points_vector,inclusive_vector,result,args.size(),
        [&](string_t sketch_data,list_entry_t split_points_data,bool inclusive_data) {

            auto sketch = datasketches::quantiles_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());

                    T *passing_points = (T *)duckdb_malloc(sizeof(T) * split_points_data.length);
                    for (idx_t i = 0; i < split_points_data.length; i++)
                    {
                        passing_points[i] = split_points_list_children_data[i + split_points_data.offset];
                    }
                    
auto pmf_result = sketch.get_PMF(passing_points, split_points_data.length, inclusive_data);
                auto current_size = ListVector::GetListSize(result);
                auto new_size = current_size + pmf_result.size();
                if (ListVector::GetListCapacity(result) < new_size)
                {
                    ListVector::Reserve(result, new_size);
                }

                auto &child_entry = ListVector::GetEntry(result);
                auto child_vals = FlatVector::GetData<double>(child_entry);
                //auto &child_validity = FlatVector::Validity(child_entry);
                for (idx_t i = 0; i < pmf_result.size(); i++)
                {
                    child_vals[current_size + i] = pmf_result[i];
                }
                ListVector::SetListSize(result, new_size);
                return list_entry_t{current_size, pmf_result.size()};
                
        });

}

    


template <class T>

static inline void DSQuantilesnormalized_rank_error(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 2);

    
        auto &sketch_vector = args.data[0];
        auto &is_pmf_vector = args.data[1];

    
        
        BinaryExecutor::Execute
        <string_t,bool,double>
        (
        sketch_vector,is_pmf_vector,result,args.size(),
        [&](string_t sketch_data,bool is_pmf_data) {

            auto sketch = datasketches::quantiles_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_normalized_rank_error(is_pmf_data);
        });

}

    


template <class T>

static inline void DSQuantilesdescribe(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 3);

    
        auto &sketch_vector = args.data[0];
        auto &include_levels_vector = args.data[1];
        auto &include_items_vector = args.data[2];

    
        
        TernaryExecutor::Execute
        <string_t,bool,bool,string_t>
        (
        sketch_vector,include_levels_vector,include_items_vector,result,args.size(),
        [&](string_t sketch_data,bool include_levels_data,bool include_items_data) {

            auto sketch = datasketches::quantiles_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return StringVector::AddString(result, sketch.to_string(include_levels_data, include_items_data));
        });

}

    


template <class T>

static inline void DSQuantilesrank(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 3);

    
        auto &sketch_vector = args.data[0];
        auto &item_vector = args.data[1];
        auto &inclusive_vector = args.data[2];

    
        
        TernaryExecutor::Execute
        <string_t,T,bool,double>
        (
        sketch_vector,item_vector,inclusive_vector,result,args.size(),
        [&](string_t sketch_data,T item_data,bool inclusive_data) {

            auto sketch = datasketches::quantiles_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_rank(item_data, inclusive_data);
        });

}

    


template <class T>

static inline void DSQuantilesquantile(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 3);

    
        auto &sketch_vector = args.data[0];
        auto &rank_vector = args.data[1];
        auto &inclusive_vector = args.data[2];

    
        
        TernaryExecutor::Execute
        <string_t,double,bool,T>
        (
        sketch_vector,rank_vector,inclusive_vector,result,args.size(),
        [&](string_t sketch_data,double rank_data,bool inclusive_data) {

            auto sketch = datasketches::quantiles_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_quantile(rank_data, inclusive_data);
        });

}

    


template <class T>

static inline void DSQuantilesn(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,uint64_t>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::quantiles_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_n();
        });

}

    


template <class T>

static inline void DSQuantilesis_estimation_mode(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,bool>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::quantiles_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.is_estimation_mode();
        });

}

    


template <class T>

static inline void DSQuantilesnum_retained(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,uint64_t>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::quantiles_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_num_retained();
        });

}

    


template <class T>

static inline void DSQuantilesmin_item(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,T>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::quantiles_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_min_item();
        });

}

    


template <class T>

static inline void DSQuantilesmax_item(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,T>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::quantiles_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_max_item();
        });

}

    


    
    template <typename T>
    auto static DSQuantilesMergeAggregate(const LogicalType &type, const LogicalType &result_type) -> AggregateFunction
    
    {
        
        return AggregateFunction::BinaryAggregate<DSQuantilesState<T>, int32_t, string_t, string_t, DSQuantilesMergeOperation>(
            LogicalType::INTEGER, result_type, result_type);
        
    }



    template <typename T>
    auto static DSQuantilesCreateAggregate(const LogicalType &type, const LogicalType &result_type) -> AggregateFunction
    {
        
        return AggregateFunction::BinaryAggregate<DSQuantilesState<T>, int32_t, T, string_t, DSQuantilesCreateOperation>(
            LogicalType::INTEGER, type, result_type);
        
    }


  void LoadQuantilesSketch(DatabaseInstance &instance) {
      auto &system_catalog = Catalog::GetSystemCatalog(instance);
      auto data = CatalogTransaction::GetSystemTransaction(instance);


      
        std::unordered_map<LogicalTypeId, LogicalType> sketch_map_types;
        
            sketch_map_types.insert({ LogicalType::TINYINT, CreateQuantilesSketchType(instance, LogicalType(LogicalType::TINYINT))});
            sketch_map_types.insert({ LogicalType::SMALLINT, CreateQuantilesSketchType(instance, LogicalType(LogicalType::SMALLINT))});
            sketch_map_types.insert({ LogicalType::INTEGER, CreateQuantilesSketchType(instance, LogicalType(LogicalType::INTEGER))});
            sketch_map_types.insert({ LogicalType::BIGINT, CreateQuantilesSketchType(instance, LogicalType(LogicalType::BIGINT))});
            sketch_map_types.insert({ LogicalType::FLOAT, CreateQuantilesSketchType(instance, LogicalType(LogicalType::FLOAT))});
            sketch_map_types.insert({ LogicalType::DOUBLE, CreateQuantilesSketchType(instance, LogicalType(LogicalType::DOUBLE))});
            sketch_map_types.insert({ LogicalType::UTINYINT, CreateQuantilesSketchType(instance, LogicalType(LogicalType::UTINYINT))});
            sketch_map_types.insert({ LogicalType::USMALLINT, CreateQuantilesSketchType(instance, LogicalType(LogicalType::USMALLINT))});
            sketch_map_types.insert({ LogicalType::UINTEGER, CreateQuantilesSketchType(instance, LogicalType(LogicalType::UINTEGER))});
            sketch_map_types.insert({ LogicalType::UBIGINT, CreateQuantilesSketchType(instance, LogicalType(LogicalType::UBIGINT))});
      


      
      {
        ScalarFunctionSet fs("datasketch_quantiles_is_empty");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::BOOLEAN
                ,    DSQuantilesis_empty<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::BOOLEAN
                ,    DSQuantilesis_empty<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::BOOLEAN
                ,    DSQuantilesis_empty<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::BOOLEAN
                ,    DSQuantilesis_empty<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::BOOLEAN
                ,    DSQuantilesis_empty<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::BOOLEAN
                ,    DSQuantilesis_empty<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::BOOLEAN
                ,    DSQuantilesis_empty<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::BOOLEAN
                ,    DSQuantilesis_empty<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::BOOLEAN
                ,    DSQuantilesis_empty<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::BOOLEAN
                ,    DSQuantilesis_empty<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_quantiles_k");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::USMALLINT
                ,    DSQuantilesk<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::USMALLINT
                ,    DSQuantilesk<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::USMALLINT
                ,    DSQuantilesk<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::USMALLINT
                ,    DSQuantilesk<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::USMALLINT
                ,    DSQuantilesk<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::USMALLINT
                ,    DSQuantilesk<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::USMALLINT
                ,    DSQuantilesk<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::USMALLINT
                ,    DSQuantilesk<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::USMALLINT
                ,    DSQuantilesk<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::USMALLINT
                ,    DSQuantilesk<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_quantiles_cdf");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT],LogicalType::LIST(LogicalType::TINYINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::TINYINT)
                ,    DSQuantilescdf<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT],LogicalType::LIST(LogicalType::SMALLINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::SMALLINT)
                ,    DSQuantilescdf<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER],LogicalType::LIST(LogicalType::INTEGER),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::INTEGER)
                ,    DSQuantilescdf<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT],LogicalType::LIST(LogicalType::BIGINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::BIGINT)
                ,    DSQuantilescdf<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::LIST(LogicalType::FLOAT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::FLOAT)
                ,    DSQuantilescdf<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::LIST(LogicalType::DOUBLE),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::DOUBLE)
                ,    DSQuantilescdf<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT],LogicalType::LIST(LogicalType::UTINYINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::UTINYINT)
                ,    DSQuantilescdf<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT],LogicalType::LIST(LogicalType::USMALLINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::USMALLINT)
                ,    DSQuantilescdf<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER],LogicalType::LIST(LogicalType::UINTEGER),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::UINTEGER)
                ,    DSQuantilescdf<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT],LogicalType::LIST(LogicalType::UBIGINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::UBIGINT)
                ,    DSQuantilescdf<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_quantiles_pmf");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT],LogicalType::LIST(LogicalType::TINYINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::TINYINT)
                ,    DSQuantilespmf<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT],LogicalType::LIST(LogicalType::SMALLINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::SMALLINT)
                ,    DSQuantilespmf<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER],LogicalType::LIST(LogicalType::INTEGER),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::INTEGER)
                ,    DSQuantilespmf<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT],LogicalType::LIST(LogicalType::BIGINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::BIGINT)
                ,    DSQuantilespmf<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::LIST(LogicalType::FLOAT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::FLOAT)
                ,    DSQuantilespmf<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::LIST(LogicalType::DOUBLE),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::DOUBLE)
                ,    DSQuantilespmf<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT],LogicalType::LIST(LogicalType::UTINYINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::UTINYINT)
                ,    DSQuantilespmf<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT],LogicalType::LIST(LogicalType::USMALLINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::USMALLINT)
                ,    DSQuantilespmf<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER],LogicalType::LIST(LogicalType::UINTEGER),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::UINTEGER)
                ,    DSQuantilespmf<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT],LogicalType::LIST(LogicalType::UBIGINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::UBIGINT)
                ,    DSQuantilespmf<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_quantiles_normalized_rank_error");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesnormalized_rank_error<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesnormalized_rank_error<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesnormalized_rank_error<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesnormalized_rank_error<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesnormalized_rank_error<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesnormalized_rank_error<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesnormalized_rank_error<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesnormalized_rank_error<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesnormalized_rank_error<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesnormalized_rank_error<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_quantiles_describe");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSQuantilesdescribe<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSQuantilesdescribe<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSQuantilesdescribe<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSQuantilesdescribe<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSQuantilesdescribe<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSQuantilesdescribe<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSQuantilesdescribe<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSQuantilesdescribe<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSQuantilesdescribe<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSQuantilesdescribe<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_quantiles_rank");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT],LogicalType::TINYINT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesrank<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT],LogicalType::SMALLINT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesrank<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER],LogicalType::INTEGER,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesrank<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT],LogicalType::BIGINT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesrank<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::FLOAT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesrank<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesrank<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT],LogicalType::UTINYINT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesrank<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT],LogicalType::USMALLINT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesrank<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER],LogicalType::UINTEGER,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesrank<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT],LogicalType::UBIGINT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesrank<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_quantiles_quantile");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::TINYINT
                ,    DSQuantilesquantile<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::SMALLINT
                ,    DSQuantilesquantile<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::INTEGER
                ,    DSQuantilesquantile<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::BIGINT
                ,    DSQuantilesquantile<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::FLOAT
                ,    DSQuantilesquantile<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSQuantilesquantile<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::UTINYINT
                ,    DSQuantilesquantile<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::USMALLINT
                ,    DSQuantilesquantile<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::UINTEGER
                ,    DSQuantilesquantile<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::UBIGINT
                ,    DSQuantilesquantile<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_quantiles_n");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::UBIGINT
                ,    DSQuantilesn<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::UBIGINT
                ,    DSQuantilesn<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::UBIGINT
                ,    DSQuantilesn<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::UBIGINT
                ,    DSQuantilesn<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::UBIGINT
                ,    DSQuantilesn<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::UBIGINT
                ,    DSQuantilesn<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::UBIGINT
                ,    DSQuantilesn<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::UBIGINT
                ,    DSQuantilesn<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::UBIGINT
                ,    DSQuantilesn<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::UBIGINT
                ,    DSQuantilesn<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_quantiles_is_estimation_mode");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::BOOLEAN
                ,    DSQuantilesis_estimation_mode<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::BOOLEAN
                ,    DSQuantilesis_estimation_mode<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::BOOLEAN
                ,    DSQuantilesis_estimation_mode<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::BOOLEAN
                ,    DSQuantilesis_estimation_mode<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::BOOLEAN
                ,    DSQuantilesis_estimation_mode<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::BOOLEAN
                ,    DSQuantilesis_estimation_mode<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::BOOLEAN
                ,    DSQuantilesis_estimation_mode<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::BOOLEAN
                ,    DSQuantilesis_estimation_mode<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::BOOLEAN
                ,    DSQuantilesis_estimation_mode<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::BOOLEAN
                ,    DSQuantilesis_estimation_mode<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_quantiles_num_retained");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::UBIGINT
                ,    DSQuantilesnum_retained<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::UBIGINT
                ,    DSQuantilesnum_retained<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::UBIGINT
                ,    DSQuantilesnum_retained<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::UBIGINT
                ,    DSQuantilesnum_retained<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::UBIGINT
                ,    DSQuantilesnum_retained<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::UBIGINT
                ,    DSQuantilesnum_retained<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::UBIGINT
                ,    DSQuantilesnum_retained<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::UBIGINT
                ,    DSQuantilesnum_retained<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::UBIGINT
                ,    DSQuantilesnum_retained<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::UBIGINT
                ,    DSQuantilesnum_retained<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_quantiles_min_item");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::TINYINT
                ,    DSQuantilesmin_item<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::SMALLINT
                ,    DSQuantilesmin_item<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::INTEGER
                ,    DSQuantilesmin_item<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::BIGINT
                ,    DSQuantilesmin_item<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::FLOAT
                ,    DSQuantilesmin_item<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::DOUBLE
                ,    DSQuantilesmin_item<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::UTINYINT
                ,    DSQuantilesmin_item<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::USMALLINT
                ,    DSQuantilesmin_item<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::UINTEGER
                ,    DSQuantilesmin_item<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::UBIGINT
                ,    DSQuantilesmin_item<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_quantiles_max_item");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::TINYINT
                ,    DSQuantilesmax_item<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::SMALLINT
                ,    DSQuantilesmax_item<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::INTEGER
                ,    DSQuantilesmax_item<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::BIGINT
                ,    DSQuantilesmax_item<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::FLOAT
                ,    DSQuantilesmax_item<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::DOUBLE
                ,    DSQuantilesmax_item<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::UTINYINT
                ,    DSQuantilesmax_item<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::USMALLINT
                ,    DSQuantilesmax_item<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::UINTEGER
                ,    DSQuantilesmax_item<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::UBIGINT
                ,    DSQuantilesmax_item<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }

      // This funciton creates the sketches.
      {
      AggregateFunctionSet sketch("datasketch_quantiles");
      
        
            sketch.AddFunction(DSQuantilesCreateAggregate<int8_t>(LogicalType::TINYINT, sketch_map_types[LogicalType::TINYINT]));
            sketch.AddFunction(DSQuantilesMergeAggregate<int8_t>(LogicalType::TINYINT, sketch_map_types[LogicalType::TINYINT]));
        
        
            sketch.AddFunction(DSQuantilesCreateAggregate<int16_t>(LogicalType::SMALLINT, sketch_map_types[LogicalType::SMALLINT]));
            sketch.AddFunction(DSQuantilesMergeAggregate<int16_t>(LogicalType::SMALLINT, sketch_map_types[LogicalType::SMALLINT]));
        
        
            sketch.AddFunction(DSQuantilesCreateAggregate<int32_t>(LogicalType::INTEGER, sketch_map_types[LogicalType::INTEGER]));
            sketch.AddFunction(DSQuantilesMergeAggregate<int32_t>(LogicalType::INTEGER, sketch_map_types[LogicalType::INTEGER]));
        
        
            sketch.AddFunction(DSQuantilesCreateAggregate<int64_t>(LogicalType::BIGINT, sketch_map_types[LogicalType::BIGINT]));
            sketch.AddFunction(DSQuantilesMergeAggregate<int64_t>(LogicalType::BIGINT, sketch_map_types[LogicalType::BIGINT]));
        
        
            sketch.AddFunction(DSQuantilesCreateAggregate<float>(LogicalType::FLOAT, sketch_map_types[LogicalType::FLOAT]));
            sketch.AddFunction(DSQuantilesMergeAggregate<float>(LogicalType::FLOAT, sketch_map_types[LogicalType::FLOAT]));
        
        
            sketch.AddFunction(DSQuantilesCreateAggregate<double>(LogicalType::DOUBLE, sketch_map_types[LogicalType::DOUBLE]));
            sketch.AddFunction(DSQuantilesMergeAggregate<double>(LogicalType::DOUBLE, sketch_map_types[LogicalType::DOUBLE]));
        
        
            sketch.AddFunction(DSQuantilesCreateAggregate<uint8_t>(LogicalType::UTINYINT, sketch_map_types[LogicalType::UTINYINT]));
            sketch.AddFunction(DSQuantilesMergeAggregate<uint8_t>(LogicalType::UTINYINT, sketch_map_types[LogicalType::UTINYINT]));
        
        
            sketch.AddFunction(DSQuantilesCreateAggregate<uint16_t>(LogicalType::USMALLINT, sketch_map_types[LogicalType::USMALLINT]));
            sketch.AddFunction(DSQuantilesMergeAggregate<uint16_t>(LogicalType::USMALLINT, sketch_map_types[LogicalType::USMALLINT]));
        
        
            sketch.AddFunction(DSQuantilesCreateAggregate<uint32_t>(LogicalType::UINTEGER, sketch_map_types[LogicalType::UINTEGER]));
            sketch.AddFunction(DSQuantilesMergeAggregate<uint32_t>(LogicalType::UINTEGER, sketch_map_types[LogicalType::UINTEGER]));
        
        
            sketch.AddFunction(DSQuantilesCreateAggregate<uint64_t>(LogicalType::UBIGINT, sketch_map_types[LogicalType::UBIGINT]));
            sketch.AddFunction(DSQuantilesMergeAggregate<uint64_t>(LogicalType::UBIGINT, sketch_map_types[LogicalType::UBIGINT]));
        
      CreateAggregateFunctionInfo sketch_info(sketch);
      system_catalog.CreateFunction(data, sketch_info);
      }



      


  }
    


template <class T>

static inline void DSKLLis_empty(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,bool>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::kll_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.is_empty();
        });

}

    


template <class T>

static inline void DSKLLk(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,uint16_t>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::kll_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_k();
        });

}

    


template <class T>

static inline void DSKLLcdf(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 3);

    
        auto &sketch_vector = args.data[0];
        auto &split_points_vector = args.data[1];
        auto &inclusive_vector = args.data[2];

    
        
                    UnifiedVectorFormat unified_split_points;
                    split_points_vector.ToUnifiedFormat(args.size(), unified_split_points);

                    // auto split_points_list_entries = UnifiedVectorFormat::GetData<list_entry_t>(unified_split_points);
                    //         auto split_points_validitiy = FlatVector::Validity(split_points_vector);

                    auto &split_points_list_children = ListVector::GetEntry(split_points_vector);

                    UnifiedVectorFormat split_points_children_unified;
                    split_points_list_children.ToUnifiedFormat(args.size(), split_points_children_unified);

                    const T *split_points_list_children_data = UnifiedVectorFormat::GetData<T>(split_points_children_unified);
                    
        TernaryExecutor::Execute
        <string_t,list_entry_t,bool,list_entry_t>
        (
        sketch_vector,split_points_vector,inclusive_vector,result,args.size(),
        [&](string_t sketch_data,list_entry_t split_points_data,bool inclusive_data) {

            auto sketch = datasketches::kll_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());

                    T *passing_points = (T *)duckdb_malloc(sizeof(T) * split_points_data.length);
                    for (idx_t i = 0; i < split_points_data.length; i++)
                    {
                        passing_points[i] = split_points_list_children_data[i + split_points_data.offset];
                    }
                    
auto cdf_result = sketch.get_CDF(passing_points, split_points_data.length, inclusive_data);
                auto current_size = ListVector::GetListSize(result);
                auto new_size = current_size + cdf_result.size();
                if (ListVector::GetListCapacity(result) < new_size)
                {
                    ListVector::Reserve(result, new_size);
                }

                auto &child_entry = ListVector::GetEntry(result);
                auto child_vals = FlatVector::GetData<T>(child_entry);
                //auto &child_validity = FlatVector::Validity(child_entry);
                for (idx_t i = 0; i < cdf_result.size(); i++)
                {
                    child_vals[current_size + i] = cdf_result[i];
                }
                ListVector::SetListSize(result, new_size);
                return list_entry_t{current_size, cdf_result.size()};
                
        });

}

    


template <class T>

static inline void DSKLLpmf(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 3);

    
        auto &sketch_vector = args.data[0];
        auto &split_points_vector = args.data[1];
        auto &inclusive_vector = args.data[2];

    
        
                    UnifiedVectorFormat unified_split_points;
                    split_points_vector.ToUnifiedFormat(args.size(), unified_split_points);

                    // auto split_points_list_entries = UnifiedVectorFormat::GetData<list_entry_t>(unified_split_points);
                    //         auto split_points_validitiy = FlatVector::Validity(split_points_vector);

                    auto &split_points_list_children = ListVector::GetEntry(split_points_vector);

                    UnifiedVectorFormat split_points_children_unified;
                    split_points_list_children.ToUnifiedFormat(args.size(), split_points_children_unified);

                    const T *split_points_list_children_data = UnifiedVectorFormat::GetData<T>(split_points_children_unified);
                    
        TernaryExecutor::Execute
        <string_t,list_entry_t,bool,list_entry_t>
        (
        sketch_vector,split_points_vector,inclusive_vector,result,args.size(),
        [&](string_t sketch_data,list_entry_t split_points_data,bool inclusive_data) {

            auto sketch = datasketches::kll_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());

                    T *passing_points = (T *)duckdb_malloc(sizeof(T) * split_points_data.length);
                    for (idx_t i = 0; i < split_points_data.length; i++)
                    {
                        passing_points[i] = split_points_list_children_data[i + split_points_data.offset];
                    }
                    
auto pmf_result = sketch.get_PMF(passing_points, split_points_data.length, inclusive_data);
                auto current_size = ListVector::GetListSize(result);
                auto new_size = current_size + pmf_result.size();
                if (ListVector::GetListCapacity(result) < new_size)
                {
                    ListVector::Reserve(result, new_size);
                }

                auto &child_entry = ListVector::GetEntry(result);
                auto child_vals = FlatVector::GetData<double>(child_entry);
                //auto &child_validity = FlatVector::Validity(child_entry);
                for (idx_t i = 0; i < pmf_result.size(); i++)
                {
                    child_vals[current_size + i] = pmf_result[i];
                }
                ListVector::SetListSize(result, new_size);
                return list_entry_t{current_size, pmf_result.size()};
                
        });

}

    


template <class T>

static inline void DSKLLnormalized_rank_error(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 2);

    
        auto &sketch_vector = args.data[0];
        auto &is_pmf_vector = args.data[1];

    
        
        BinaryExecutor::Execute
        <string_t,bool,double>
        (
        sketch_vector,is_pmf_vector,result,args.size(),
        [&](string_t sketch_data,bool is_pmf_data) {

            auto sketch = datasketches::kll_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_normalized_rank_error(is_pmf_data);
        });

}

    


template <class T>

static inline void DSKLLdescribe(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 3);

    
        auto &sketch_vector = args.data[0];
        auto &include_levels_vector = args.data[1];
        auto &include_items_vector = args.data[2];

    
        
        TernaryExecutor::Execute
        <string_t,bool,bool,string_t>
        (
        sketch_vector,include_levels_vector,include_items_vector,result,args.size(),
        [&](string_t sketch_data,bool include_levels_data,bool include_items_data) {

            auto sketch = datasketches::kll_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return StringVector::AddString(result, sketch.to_string(include_levels_data, include_items_data));
        });

}

    


template <class T>

static inline void DSKLLrank(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 3);

    
        auto &sketch_vector = args.data[0];
        auto &item_vector = args.data[1];
        auto &inclusive_vector = args.data[2];

    
        
        TernaryExecutor::Execute
        <string_t,T,bool,double>
        (
        sketch_vector,item_vector,inclusive_vector,result,args.size(),
        [&](string_t sketch_data,T item_data,bool inclusive_data) {

            auto sketch = datasketches::kll_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_rank(item_data, inclusive_data);
        });

}

    


template <class T>

static inline void DSKLLquantile(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 3);

    
        auto &sketch_vector = args.data[0];
        auto &rank_vector = args.data[1];
        auto &inclusive_vector = args.data[2];

    
        
        TernaryExecutor::Execute
        <string_t,double,bool,T>
        (
        sketch_vector,rank_vector,inclusive_vector,result,args.size(),
        [&](string_t sketch_data,double rank_data,bool inclusive_data) {

            auto sketch = datasketches::kll_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_quantile(rank_data, inclusive_data);
        });

}

    


template <class T>

static inline void DSKLLn(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,uint64_t>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::kll_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_n();
        });

}

    


template <class T>

static inline void DSKLLis_estimation_mode(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,bool>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::kll_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.is_estimation_mode();
        });

}

    


template <class T>

static inline void DSKLLnum_retained(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,uint64_t>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::kll_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_num_retained();
        });

}

    


template <class T>

static inline void DSKLLmin_item(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,T>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::kll_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_min_item();
        });

}

    


template <class T>

static inline void DSKLLmax_item(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,T>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::kll_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_max_item();
        });

}

    


    
    template <typename T>
    auto static DSKLLMergeAggregate(const LogicalType &type, const LogicalType &result_type) -> AggregateFunction
    
    {
        
        return AggregateFunction::BinaryAggregate<DSKLLState<T>, int32_t, string_t, string_t, DSQuantilesMergeOperation>(
            LogicalType::INTEGER, result_type, result_type);
        
    }



    template <typename T>
    auto static DSKLLCreateAggregate(const LogicalType &type, const LogicalType &result_type) -> AggregateFunction
    {
        
        return AggregateFunction::BinaryAggregate<DSKLLState<T>, int32_t, T, string_t, DSQuantilesCreateOperation>(
            LogicalType::INTEGER, type, result_type);
        
    }


  void LoadKLLSketch(DatabaseInstance &instance) {
      auto &system_catalog = Catalog::GetSystemCatalog(instance);
      auto data = CatalogTransaction::GetSystemTransaction(instance);


      
        std::unordered_map<LogicalTypeId, LogicalType> sketch_map_types;
        
            sketch_map_types.insert({ LogicalType::TINYINT, CreateKLLSketchType(instance, LogicalType(LogicalType::TINYINT))});
            sketch_map_types.insert({ LogicalType::SMALLINT, CreateKLLSketchType(instance, LogicalType(LogicalType::SMALLINT))});
            sketch_map_types.insert({ LogicalType::INTEGER, CreateKLLSketchType(instance, LogicalType(LogicalType::INTEGER))});
            sketch_map_types.insert({ LogicalType::BIGINT, CreateKLLSketchType(instance, LogicalType(LogicalType::BIGINT))});
            sketch_map_types.insert({ LogicalType::FLOAT, CreateKLLSketchType(instance, LogicalType(LogicalType::FLOAT))});
            sketch_map_types.insert({ LogicalType::DOUBLE, CreateKLLSketchType(instance, LogicalType(LogicalType::DOUBLE))});
            sketch_map_types.insert({ LogicalType::UTINYINT, CreateKLLSketchType(instance, LogicalType(LogicalType::UTINYINT))});
            sketch_map_types.insert({ LogicalType::USMALLINT, CreateKLLSketchType(instance, LogicalType(LogicalType::USMALLINT))});
            sketch_map_types.insert({ LogicalType::UINTEGER, CreateKLLSketchType(instance, LogicalType(LogicalType::UINTEGER))});
            sketch_map_types.insert({ LogicalType::UBIGINT, CreateKLLSketchType(instance, LogicalType(LogicalType::UBIGINT))});
      


      
      {
        ScalarFunctionSet fs("datasketch_kll_is_empty");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::BOOLEAN
                ,    DSKLLis_empty<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::BOOLEAN
                ,    DSKLLis_empty<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::BOOLEAN
                ,    DSKLLis_empty<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::BOOLEAN
                ,    DSKLLis_empty<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::BOOLEAN
                ,    DSKLLis_empty<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::BOOLEAN
                ,    DSKLLis_empty<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::BOOLEAN
                ,    DSKLLis_empty<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::BOOLEAN
                ,    DSKLLis_empty<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::BOOLEAN
                ,    DSKLLis_empty<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::BOOLEAN
                ,    DSKLLis_empty<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_kll_k");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::USMALLINT
                ,    DSKLLk<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::USMALLINT
                ,    DSKLLk<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::USMALLINT
                ,    DSKLLk<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::USMALLINT
                ,    DSKLLk<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::USMALLINT
                ,    DSKLLk<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::USMALLINT
                ,    DSKLLk<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::USMALLINT
                ,    DSKLLk<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::USMALLINT
                ,    DSKLLk<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::USMALLINT
                ,    DSKLLk<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::USMALLINT
                ,    DSKLLk<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_kll_cdf");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT],LogicalType::LIST(LogicalType::TINYINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::TINYINT)
                ,    DSKLLcdf<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT],LogicalType::LIST(LogicalType::SMALLINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::SMALLINT)
                ,    DSKLLcdf<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER],LogicalType::LIST(LogicalType::INTEGER),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::INTEGER)
                ,    DSKLLcdf<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT],LogicalType::LIST(LogicalType::BIGINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::BIGINT)
                ,    DSKLLcdf<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::LIST(LogicalType::FLOAT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::FLOAT)
                ,    DSKLLcdf<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::LIST(LogicalType::DOUBLE),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::DOUBLE)
                ,    DSKLLcdf<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT],LogicalType::LIST(LogicalType::UTINYINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::UTINYINT)
                ,    DSKLLcdf<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT],LogicalType::LIST(LogicalType::USMALLINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::USMALLINT)
                ,    DSKLLcdf<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER],LogicalType::LIST(LogicalType::UINTEGER),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::UINTEGER)
                ,    DSKLLcdf<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT],LogicalType::LIST(LogicalType::UBIGINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::UBIGINT)
                ,    DSKLLcdf<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_kll_pmf");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT],LogicalType::LIST(LogicalType::TINYINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::TINYINT)
                ,    DSKLLpmf<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT],LogicalType::LIST(LogicalType::SMALLINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::SMALLINT)
                ,    DSKLLpmf<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER],LogicalType::LIST(LogicalType::INTEGER),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::INTEGER)
                ,    DSKLLpmf<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT],LogicalType::LIST(LogicalType::BIGINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::BIGINT)
                ,    DSKLLpmf<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::LIST(LogicalType::FLOAT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::FLOAT)
                ,    DSKLLpmf<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::LIST(LogicalType::DOUBLE),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::DOUBLE)
                ,    DSKLLpmf<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT],LogicalType::LIST(LogicalType::UTINYINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::UTINYINT)
                ,    DSKLLpmf<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT],LogicalType::LIST(LogicalType::USMALLINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::USMALLINT)
                ,    DSKLLpmf<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER],LogicalType::LIST(LogicalType::UINTEGER),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::UINTEGER)
                ,    DSKLLpmf<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT],LogicalType::LIST(LogicalType::UBIGINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::UBIGINT)
                ,    DSKLLpmf<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_kll_normalized_rank_error");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLnormalized_rank_error<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLnormalized_rank_error<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLnormalized_rank_error<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLnormalized_rank_error<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLnormalized_rank_error<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLnormalized_rank_error<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLnormalized_rank_error<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLnormalized_rank_error<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLnormalized_rank_error<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT],LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLnormalized_rank_error<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_kll_describe");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSKLLdescribe<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSKLLdescribe<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSKLLdescribe<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSKLLdescribe<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSKLLdescribe<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSKLLdescribe<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSKLLdescribe<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSKLLdescribe<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSKLLdescribe<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSKLLdescribe<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_kll_rank");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT],LogicalType::TINYINT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLrank<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT],LogicalType::SMALLINT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLrank<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER],LogicalType::INTEGER,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLrank<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT],LogicalType::BIGINT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLrank<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::FLOAT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLrank<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLrank<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT],LogicalType::UTINYINT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLrank<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT],LogicalType::USMALLINT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLrank<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER],LogicalType::UINTEGER,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLrank<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT],LogicalType::UBIGINT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLrank<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_kll_quantile");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::TINYINT
                ,    DSKLLquantile<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::SMALLINT
                ,    DSKLLquantile<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::INTEGER
                ,    DSKLLquantile<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::BIGINT
                ,    DSKLLquantile<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::FLOAT
                ,    DSKLLquantile<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSKLLquantile<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::UTINYINT
                ,    DSKLLquantile<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::USMALLINT
                ,    DSKLLquantile<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::UINTEGER
                ,    DSKLLquantile<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::UBIGINT
                ,    DSKLLquantile<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_kll_n");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::UBIGINT
                ,    DSKLLn<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::UBIGINT
                ,    DSKLLn<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::UBIGINT
                ,    DSKLLn<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::UBIGINT
                ,    DSKLLn<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::UBIGINT
                ,    DSKLLn<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::UBIGINT
                ,    DSKLLn<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::UBIGINT
                ,    DSKLLn<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::UBIGINT
                ,    DSKLLn<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::UBIGINT
                ,    DSKLLn<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::UBIGINT
                ,    DSKLLn<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_kll_is_estimation_mode");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::BOOLEAN
                ,    DSKLLis_estimation_mode<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::BOOLEAN
                ,    DSKLLis_estimation_mode<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::BOOLEAN
                ,    DSKLLis_estimation_mode<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::BOOLEAN
                ,    DSKLLis_estimation_mode<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::BOOLEAN
                ,    DSKLLis_estimation_mode<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::BOOLEAN
                ,    DSKLLis_estimation_mode<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::BOOLEAN
                ,    DSKLLis_estimation_mode<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::BOOLEAN
                ,    DSKLLis_estimation_mode<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::BOOLEAN
                ,    DSKLLis_estimation_mode<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::BOOLEAN
                ,    DSKLLis_estimation_mode<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_kll_num_retained");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::UBIGINT
                ,    DSKLLnum_retained<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::UBIGINT
                ,    DSKLLnum_retained<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::UBIGINT
                ,    DSKLLnum_retained<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::UBIGINT
                ,    DSKLLnum_retained<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::UBIGINT
                ,    DSKLLnum_retained<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::UBIGINT
                ,    DSKLLnum_retained<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::UBIGINT
                ,    DSKLLnum_retained<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::UBIGINT
                ,    DSKLLnum_retained<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::UBIGINT
                ,    DSKLLnum_retained<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::UBIGINT
                ,    DSKLLnum_retained<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_kll_min_item");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::TINYINT
                ,    DSKLLmin_item<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::SMALLINT
                ,    DSKLLmin_item<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::INTEGER
                ,    DSKLLmin_item<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::BIGINT
                ,    DSKLLmin_item<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::FLOAT
                ,    DSKLLmin_item<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::DOUBLE
                ,    DSKLLmin_item<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::UTINYINT
                ,    DSKLLmin_item<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::USMALLINT
                ,    DSKLLmin_item<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::UINTEGER
                ,    DSKLLmin_item<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::UBIGINT
                ,    DSKLLmin_item<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_kll_max_item");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::TINYINT
                ,    DSKLLmax_item<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::SMALLINT
                ,    DSKLLmax_item<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::INTEGER
                ,    DSKLLmax_item<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::BIGINT
                ,    DSKLLmax_item<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::FLOAT
                ,    DSKLLmax_item<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::DOUBLE
                ,    DSKLLmax_item<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::UTINYINT
                ,    DSKLLmax_item<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::USMALLINT
                ,    DSKLLmax_item<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::UINTEGER
                ,    DSKLLmax_item<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::UBIGINT
                ,    DSKLLmax_item<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }

      // This funciton creates the sketches.
      {
      AggregateFunctionSet sketch("datasketch_kll");
      
        
            sketch.AddFunction(DSKLLCreateAggregate<int8_t>(LogicalType::TINYINT, sketch_map_types[LogicalType::TINYINT]));
            sketch.AddFunction(DSKLLMergeAggregate<int8_t>(LogicalType::TINYINT, sketch_map_types[LogicalType::TINYINT]));
        
        
            sketch.AddFunction(DSKLLCreateAggregate<int16_t>(LogicalType::SMALLINT, sketch_map_types[LogicalType::SMALLINT]));
            sketch.AddFunction(DSKLLMergeAggregate<int16_t>(LogicalType::SMALLINT, sketch_map_types[LogicalType::SMALLINT]));
        
        
            sketch.AddFunction(DSKLLCreateAggregate<int32_t>(LogicalType::INTEGER, sketch_map_types[LogicalType::INTEGER]));
            sketch.AddFunction(DSKLLMergeAggregate<int32_t>(LogicalType::INTEGER, sketch_map_types[LogicalType::INTEGER]));
        
        
            sketch.AddFunction(DSKLLCreateAggregate<int64_t>(LogicalType::BIGINT, sketch_map_types[LogicalType::BIGINT]));
            sketch.AddFunction(DSKLLMergeAggregate<int64_t>(LogicalType::BIGINT, sketch_map_types[LogicalType::BIGINT]));
        
        
            sketch.AddFunction(DSKLLCreateAggregate<float>(LogicalType::FLOAT, sketch_map_types[LogicalType::FLOAT]));
            sketch.AddFunction(DSKLLMergeAggregate<float>(LogicalType::FLOAT, sketch_map_types[LogicalType::FLOAT]));
        
        
            sketch.AddFunction(DSKLLCreateAggregate<double>(LogicalType::DOUBLE, sketch_map_types[LogicalType::DOUBLE]));
            sketch.AddFunction(DSKLLMergeAggregate<double>(LogicalType::DOUBLE, sketch_map_types[LogicalType::DOUBLE]));
        
        
            sketch.AddFunction(DSKLLCreateAggregate<uint8_t>(LogicalType::UTINYINT, sketch_map_types[LogicalType::UTINYINT]));
            sketch.AddFunction(DSKLLMergeAggregate<uint8_t>(LogicalType::UTINYINT, sketch_map_types[LogicalType::UTINYINT]));
        
        
            sketch.AddFunction(DSKLLCreateAggregate<uint16_t>(LogicalType::USMALLINT, sketch_map_types[LogicalType::USMALLINT]));
            sketch.AddFunction(DSKLLMergeAggregate<uint16_t>(LogicalType::USMALLINT, sketch_map_types[LogicalType::USMALLINT]));
        
        
            sketch.AddFunction(DSKLLCreateAggregate<uint32_t>(LogicalType::UINTEGER, sketch_map_types[LogicalType::UINTEGER]));
            sketch.AddFunction(DSKLLMergeAggregate<uint32_t>(LogicalType::UINTEGER, sketch_map_types[LogicalType::UINTEGER]));
        
        
            sketch.AddFunction(DSKLLCreateAggregate<uint64_t>(LogicalType::UBIGINT, sketch_map_types[LogicalType::UBIGINT]));
            sketch.AddFunction(DSKLLMergeAggregate<uint64_t>(LogicalType::UBIGINT, sketch_map_types[LogicalType::UBIGINT]));
        
      CreateAggregateFunctionInfo sketch_info(sketch);
      system_catalog.CreateFunction(data, sketch_info);
      }



      


  }
    


template <class T>

static inline void DSREQis_empty(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,bool>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::req_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.is_empty();
        });

}

    


template <class T>

static inline void DSREQk(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,uint16_t>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::req_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_k();
        });

}

    


template <class T>

static inline void DSREQcdf(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 3);

    
        auto &sketch_vector = args.data[0];
        auto &split_points_vector = args.data[1];
        auto &inclusive_vector = args.data[2];

    
        
                    UnifiedVectorFormat unified_split_points;
                    split_points_vector.ToUnifiedFormat(args.size(), unified_split_points);

                    // auto split_points_list_entries = UnifiedVectorFormat::GetData<list_entry_t>(unified_split_points);
                    //         auto split_points_validitiy = FlatVector::Validity(split_points_vector);

                    auto &split_points_list_children = ListVector::GetEntry(split_points_vector);

                    UnifiedVectorFormat split_points_children_unified;
                    split_points_list_children.ToUnifiedFormat(args.size(), split_points_children_unified);

                    const T *split_points_list_children_data = UnifiedVectorFormat::GetData<T>(split_points_children_unified);
                    
        TernaryExecutor::Execute
        <string_t,list_entry_t,bool,list_entry_t>
        (
        sketch_vector,split_points_vector,inclusive_vector,result,args.size(),
        [&](string_t sketch_data,list_entry_t split_points_data,bool inclusive_data) {

            auto sketch = datasketches::req_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());

                    T *passing_points = (T *)duckdb_malloc(sizeof(T) * split_points_data.length);
                    for (idx_t i = 0; i < split_points_data.length; i++)
                    {
                        passing_points[i] = split_points_list_children_data[i + split_points_data.offset];
                    }
                    
auto cdf_result = sketch.get_CDF(passing_points, split_points_data.length, inclusive_data);
                auto current_size = ListVector::GetListSize(result);
                auto new_size = current_size + cdf_result.size();
                if (ListVector::GetListCapacity(result) < new_size)
                {
                    ListVector::Reserve(result, new_size);
                }

                auto &child_entry = ListVector::GetEntry(result);
                auto child_vals = FlatVector::GetData<T>(child_entry);
                //auto &child_validity = FlatVector::Validity(child_entry);
                for (idx_t i = 0; i < cdf_result.size(); i++)
                {
                    child_vals[current_size + i] = cdf_result[i];
                }
                ListVector::SetListSize(result, new_size);
                return list_entry_t{current_size, cdf_result.size()};
                
        });

}

    


template <class T>

static inline void DSREQpmf(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 3);

    
        auto &sketch_vector = args.data[0];
        auto &split_points_vector = args.data[1];
        auto &inclusive_vector = args.data[2];

    
        
                    UnifiedVectorFormat unified_split_points;
                    split_points_vector.ToUnifiedFormat(args.size(), unified_split_points);

                    // auto split_points_list_entries = UnifiedVectorFormat::GetData<list_entry_t>(unified_split_points);
                    //         auto split_points_validitiy = FlatVector::Validity(split_points_vector);

                    auto &split_points_list_children = ListVector::GetEntry(split_points_vector);

                    UnifiedVectorFormat split_points_children_unified;
                    split_points_list_children.ToUnifiedFormat(args.size(), split_points_children_unified);

                    const T *split_points_list_children_data = UnifiedVectorFormat::GetData<T>(split_points_children_unified);
                    
        TernaryExecutor::Execute
        <string_t,list_entry_t,bool,list_entry_t>
        (
        sketch_vector,split_points_vector,inclusive_vector,result,args.size(),
        [&](string_t sketch_data,list_entry_t split_points_data,bool inclusive_data) {

            auto sketch = datasketches::req_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());

                    T *passing_points = (T *)duckdb_malloc(sizeof(T) * split_points_data.length);
                    for (idx_t i = 0; i < split_points_data.length; i++)
                    {
                        passing_points[i] = split_points_list_children_data[i + split_points_data.offset];
                    }
                    
auto pmf_result = sketch.get_PMF(passing_points, split_points_data.length, inclusive_data);
                auto current_size = ListVector::GetListSize(result);
                auto new_size = current_size + pmf_result.size();
                if (ListVector::GetListCapacity(result) < new_size)
                {
                    ListVector::Reserve(result, new_size);
                }

                auto &child_entry = ListVector::GetEntry(result);
                auto child_vals = FlatVector::GetData<double>(child_entry);
                //auto &child_validity = FlatVector::Validity(child_entry);
                for (idx_t i = 0; i < pmf_result.size(); i++)
                {
                    child_vals[current_size + i] = pmf_result[i];
                }
                ListVector::SetListSize(result, new_size);
                return list_entry_t{current_size, pmf_result.size()};
                
        });

}

    


template <class T>

static inline void DSREQdescribe(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 3);

    
        auto &sketch_vector = args.data[0];
        auto &include_levels_vector = args.data[1];
        auto &include_items_vector = args.data[2];

    
        
        TernaryExecutor::Execute
        <string_t,bool,bool,string_t>
        (
        sketch_vector,include_levels_vector,include_items_vector,result,args.size(),
        [&](string_t sketch_data,bool include_levels_data,bool include_items_data) {

            auto sketch = datasketches::req_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return StringVector::AddString(result, sketch.to_string(include_levels_data, include_items_data));
        });

}

    


template <class T>

static inline void DSREQrank(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 3);

    
        auto &sketch_vector = args.data[0];
        auto &item_vector = args.data[1];
        auto &inclusive_vector = args.data[2];

    
        
        TernaryExecutor::Execute
        <string_t,T,bool,double>
        (
        sketch_vector,item_vector,inclusive_vector,result,args.size(),
        [&](string_t sketch_data,T item_data,bool inclusive_data) {

            auto sketch = datasketches::req_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_rank(item_data, inclusive_data);
        });

}

    


template <class T>

static inline void DSREQquantile(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 3);

    
        auto &sketch_vector = args.data[0];
        auto &rank_vector = args.data[1];
        auto &inclusive_vector = args.data[2];

    
        
        TernaryExecutor::Execute
        <string_t,double,bool,T>
        (
        sketch_vector,rank_vector,inclusive_vector,result,args.size(),
        [&](string_t sketch_data,double rank_data,bool inclusive_data) {

            auto sketch = datasketches::req_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_quantile(rank_data, inclusive_data);
        });

}

    


template <class T>

static inline void DSREQn(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,uint64_t>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::req_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_n();
        });

}

    


template <class T>

static inline void DSREQis_estimation_mode(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,bool>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::req_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.is_estimation_mode();
        });

}

    


template <class T>

static inline void DSREQnum_retained(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,uint64_t>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::req_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_num_retained();
        });

}

    


template <class T>

static inline void DSREQmin_item(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,T>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::req_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_min_item();
        });

}

    


template <class T>

static inline void DSREQmax_item(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,T>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::req_sketch<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_max_item();
        });

}

    


    
    template <typename T>
    auto static DSREQMergeAggregate(const LogicalType &type, const LogicalType &result_type) -> AggregateFunction
    
    {
        
        return AggregateFunction::BinaryAggregate<DSREQState<T>, int32_t, string_t, string_t, DSQuantilesMergeOperation>(
            LogicalType::INTEGER, result_type, result_type);
        
    }



    template <typename T>
    auto static DSREQCreateAggregate(const LogicalType &type, const LogicalType &result_type) -> AggregateFunction
    {
        
        return AggregateFunction::BinaryAggregate<DSREQState<T>, int32_t, T, string_t, DSQuantilesCreateOperation>(
            LogicalType::INTEGER, type, result_type);
        
    }


  void LoadREQSketch(DatabaseInstance &instance) {
      auto &system_catalog = Catalog::GetSystemCatalog(instance);
      auto data = CatalogTransaction::GetSystemTransaction(instance);


      
        std::unordered_map<LogicalTypeId, LogicalType> sketch_map_types;
        
            sketch_map_types.insert({ LogicalType::TINYINT, CreateREQSketchType(instance, LogicalType(LogicalType::TINYINT))});
            sketch_map_types.insert({ LogicalType::SMALLINT, CreateREQSketchType(instance, LogicalType(LogicalType::SMALLINT))});
            sketch_map_types.insert({ LogicalType::INTEGER, CreateREQSketchType(instance, LogicalType(LogicalType::INTEGER))});
            sketch_map_types.insert({ LogicalType::BIGINT, CreateREQSketchType(instance, LogicalType(LogicalType::BIGINT))});
            sketch_map_types.insert({ LogicalType::FLOAT, CreateREQSketchType(instance, LogicalType(LogicalType::FLOAT))});
            sketch_map_types.insert({ LogicalType::DOUBLE, CreateREQSketchType(instance, LogicalType(LogicalType::DOUBLE))});
            sketch_map_types.insert({ LogicalType::UTINYINT, CreateREQSketchType(instance, LogicalType(LogicalType::UTINYINT))});
            sketch_map_types.insert({ LogicalType::USMALLINT, CreateREQSketchType(instance, LogicalType(LogicalType::USMALLINT))});
            sketch_map_types.insert({ LogicalType::UINTEGER, CreateREQSketchType(instance, LogicalType(LogicalType::UINTEGER))});
            sketch_map_types.insert({ LogicalType::UBIGINT, CreateREQSketchType(instance, LogicalType(LogicalType::UBIGINT))});
      


      
      {
        ScalarFunctionSet fs("datasketch_req_is_empty");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::BOOLEAN
                ,    DSREQis_empty<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::BOOLEAN
                ,    DSREQis_empty<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::BOOLEAN
                ,    DSREQis_empty<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::BOOLEAN
                ,    DSREQis_empty<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::BOOLEAN
                ,    DSREQis_empty<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::BOOLEAN
                ,    DSREQis_empty<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::BOOLEAN
                ,    DSREQis_empty<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::BOOLEAN
                ,    DSREQis_empty<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::BOOLEAN
                ,    DSREQis_empty<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::BOOLEAN
                ,    DSREQis_empty<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_req_k");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::USMALLINT
                ,    DSREQk<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::USMALLINT
                ,    DSREQk<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::USMALLINT
                ,    DSREQk<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::USMALLINT
                ,    DSREQk<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::USMALLINT
                ,    DSREQk<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::USMALLINT
                ,    DSREQk<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::USMALLINT
                ,    DSREQk<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::USMALLINT
                ,    DSREQk<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::USMALLINT
                ,    DSREQk<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::USMALLINT
                ,    DSREQk<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_req_cdf");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT],LogicalType::LIST(LogicalType::TINYINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::TINYINT)
                ,    DSREQcdf<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT],LogicalType::LIST(LogicalType::SMALLINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::SMALLINT)
                ,    DSREQcdf<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER],LogicalType::LIST(LogicalType::INTEGER),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::INTEGER)
                ,    DSREQcdf<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT],LogicalType::LIST(LogicalType::BIGINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::BIGINT)
                ,    DSREQcdf<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::LIST(LogicalType::FLOAT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::FLOAT)
                ,    DSREQcdf<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::LIST(LogicalType::DOUBLE),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::DOUBLE)
                ,    DSREQcdf<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT],LogicalType::LIST(LogicalType::UTINYINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::UTINYINT)
                ,    DSREQcdf<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT],LogicalType::LIST(LogicalType::USMALLINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::USMALLINT)
                ,    DSREQcdf<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER],LogicalType::LIST(LogicalType::UINTEGER),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::UINTEGER)
                ,    DSREQcdf<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT],LogicalType::LIST(LogicalType::UBIGINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::UBIGINT)
                ,    DSREQcdf<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_req_pmf");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT],LogicalType::LIST(LogicalType::TINYINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::TINYINT)
                ,    DSREQpmf<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT],LogicalType::LIST(LogicalType::SMALLINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::SMALLINT)
                ,    DSREQpmf<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER],LogicalType::LIST(LogicalType::INTEGER),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::INTEGER)
                ,    DSREQpmf<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT],LogicalType::LIST(LogicalType::BIGINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::BIGINT)
                ,    DSREQpmf<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::LIST(LogicalType::FLOAT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::FLOAT)
                ,    DSREQpmf<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::LIST(LogicalType::DOUBLE),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::DOUBLE)
                ,    DSREQpmf<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT],LogicalType::LIST(LogicalType::UTINYINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::UTINYINT)
                ,    DSREQpmf<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT],LogicalType::LIST(LogicalType::USMALLINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::USMALLINT)
                ,    DSREQpmf<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER],LogicalType::LIST(LogicalType::UINTEGER),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::UINTEGER)
                ,    DSREQpmf<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT],LogicalType::LIST(LogicalType::UBIGINT),LogicalType::BOOLEAN},LogicalType::LIST(LogicalType::UBIGINT)
                ,    DSREQpmf<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_req_describe");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSREQdescribe<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSREQdescribe<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSREQdescribe<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSREQdescribe<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSREQdescribe<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSREQdescribe<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSREQdescribe<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSREQdescribe<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSREQdescribe<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT],LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSREQdescribe<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_req_rank");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT],LogicalType::TINYINT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSREQrank<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT],LogicalType::SMALLINT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSREQrank<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER],LogicalType::INTEGER,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSREQrank<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT],LogicalType::BIGINT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSREQrank<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::FLOAT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSREQrank<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSREQrank<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT],LogicalType::UTINYINT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSREQrank<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT],LogicalType::USMALLINT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSREQrank<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER],LogicalType::UINTEGER,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSREQrank<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT],LogicalType::UBIGINT,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSREQrank<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_req_quantile");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::TINYINT
                ,    DSREQquantile<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::SMALLINT
                ,    DSREQquantile<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::INTEGER
                ,    DSREQquantile<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::BIGINT
                ,    DSREQquantile<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::FLOAT
                ,    DSREQquantile<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::DOUBLE
                ,    DSREQquantile<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::UTINYINT
                ,    DSREQquantile<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::USMALLINT
                ,    DSREQquantile<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::UINTEGER
                ,    DSREQquantile<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT],LogicalType::DOUBLE,LogicalType::BOOLEAN},LogicalType::UBIGINT
                ,    DSREQquantile<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_req_n");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::UBIGINT
                ,    DSREQn<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::UBIGINT
                ,    DSREQn<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::UBIGINT
                ,    DSREQn<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::UBIGINT
                ,    DSREQn<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::UBIGINT
                ,    DSREQn<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::UBIGINT
                ,    DSREQn<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::UBIGINT
                ,    DSREQn<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::UBIGINT
                ,    DSREQn<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::UBIGINT
                ,    DSREQn<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::UBIGINT
                ,    DSREQn<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_req_is_estimation_mode");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::BOOLEAN
                ,    DSREQis_estimation_mode<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::BOOLEAN
                ,    DSREQis_estimation_mode<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::BOOLEAN
                ,    DSREQis_estimation_mode<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::BOOLEAN
                ,    DSREQis_estimation_mode<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::BOOLEAN
                ,    DSREQis_estimation_mode<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::BOOLEAN
                ,    DSREQis_estimation_mode<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::BOOLEAN
                ,    DSREQis_estimation_mode<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::BOOLEAN
                ,    DSREQis_estimation_mode<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::BOOLEAN
                ,    DSREQis_estimation_mode<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::BOOLEAN
                ,    DSREQis_estimation_mode<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_req_num_retained");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::UBIGINT
                ,    DSREQnum_retained<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::UBIGINT
                ,    DSREQnum_retained<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::UBIGINT
                ,    DSREQnum_retained<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::UBIGINT
                ,    DSREQnum_retained<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::UBIGINT
                ,    DSREQnum_retained<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::UBIGINT
                ,    DSREQnum_retained<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::UBIGINT
                ,    DSREQnum_retained<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::UBIGINT
                ,    DSREQnum_retained<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::UBIGINT
                ,    DSREQnum_retained<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::UBIGINT
                ,    DSREQnum_retained<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_req_min_item");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::TINYINT
                ,    DSREQmin_item<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::SMALLINT
                ,    DSREQmin_item<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::INTEGER
                ,    DSREQmin_item<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::BIGINT
                ,    DSREQmin_item<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::FLOAT
                ,    DSREQmin_item<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::DOUBLE
                ,    DSREQmin_item<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::UTINYINT
                ,    DSREQmin_item<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::USMALLINT
                ,    DSREQmin_item<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::UINTEGER
                ,    DSREQmin_item<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::UBIGINT
                ,    DSREQmin_item<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_req_max_item");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::TINYINT]},LogicalType::TINYINT
                ,    DSREQmax_item<int8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::SMALLINT]},LogicalType::SMALLINT
                ,    DSREQmax_item<int16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::INTEGER]},LogicalType::INTEGER
                ,    DSREQmax_item<int32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::BIGINT]},LogicalType::BIGINT
                ,    DSREQmax_item<int64_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::FLOAT
                ,    DSREQmax_item<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::DOUBLE
                ,    DSREQmax_item<double>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UTINYINT]},LogicalType::UTINYINT
                ,    DSREQmax_item<uint8_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::USMALLINT]},LogicalType::USMALLINT
                ,    DSREQmax_item<uint16_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UINTEGER]},LogicalType::UINTEGER
                ,    DSREQmax_item<uint32_t>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::UBIGINT]},LogicalType::UBIGINT
                ,    DSREQmax_item<uint64_t>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }

      // This funciton creates the sketches.
      {
      AggregateFunctionSet sketch("datasketch_req");
      
        
            sketch.AddFunction(DSREQCreateAggregate<int8_t>(LogicalType::TINYINT, sketch_map_types[LogicalType::TINYINT]));
            sketch.AddFunction(DSREQMergeAggregate<int8_t>(LogicalType::TINYINT, sketch_map_types[LogicalType::TINYINT]));
        
        
            sketch.AddFunction(DSREQCreateAggregate<int16_t>(LogicalType::SMALLINT, sketch_map_types[LogicalType::SMALLINT]));
            sketch.AddFunction(DSREQMergeAggregate<int16_t>(LogicalType::SMALLINT, sketch_map_types[LogicalType::SMALLINT]));
        
        
            sketch.AddFunction(DSREQCreateAggregate<int32_t>(LogicalType::INTEGER, sketch_map_types[LogicalType::INTEGER]));
            sketch.AddFunction(DSREQMergeAggregate<int32_t>(LogicalType::INTEGER, sketch_map_types[LogicalType::INTEGER]));
        
        
            sketch.AddFunction(DSREQCreateAggregate<int64_t>(LogicalType::BIGINT, sketch_map_types[LogicalType::BIGINT]));
            sketch.AddFunction(DSREQMergeAggregate<int64_t>(LogicalType::BIGINT, sketch_map_types[LogicalType::BIGINT]));
        
        
            sketch.AddFunction(DSREQCreateAggregate<float>(LogicalType::FLOAT, sketch_map_types[LogicalType::FLOAT]));
            sketch.AddFunction(DSREQMergeAggregate<float>(LogicalType::FLOAT, sketch_map_types[LogicalType::FLOAT]));
        
        
            sketch.AddFunction(DSREQCreateAggregate<double>(LogicalType::DOUBLE, sketch_map_types[LogicalType::DOUBLE]));
            sketch.AddFunction(DSREQMergeAggregate<double>(LogicalType::DOUBLE, sketch_map_types[LogicalType::DOUBLE]));
        
        
            sketch.AddFunction(DSREQCreateAggregate<uint8_t>(LogicalType::UTINYINT, sketch_map_types[LogicalType::UTINYINT]));
            sketch.AddFunction(DSREQMergeAggregate<uint8_t>(LogicalType::UTINYINT, sketch_map_types[LogicalType::UTINYINT]));
        
        
            sketch.AddFunction(DSREQCreateAggregate<uint16_t>(LogicalType::USMALLINT, sketch_map_types[LogicalType::USMALLINT]));
            sketch.AddFunction(DSREQMergeAggregate<uint16_t>(LogicalType::USMALLINT, sketch_map_types[LogicalType::USMALLINT]));
        
        
            sketch.AddFunction(DSREQCreateAggregate<uint32_t>(LogicalType::UINTEGER, sketch_map_types[LogicalType::UINTEGER]));
            sketch.AddFunction(DSREQMergeAggregate<uint32_t>(LogicalType::UINTEGER, sketch_map_types[LogicalType::UINTEGER]));
        
        
            sketch.AddFunction(DSREQCreateAggregate<uint64_t>(LogicalType::UBIGINT, sketch_map_types[LogicalType::UBIGINT]));
            sketch.AddFunction(DSREQMergeAggregate<uint64_t>(LogicalType::UBIGINT, sketch_map_types[LogicalType::UBIGINT]));
        
      CreateAggregateFunctionInfo sketch_info(sketch);
      system_catalog.CreateFunction(data, sketch_info);
      }



      


  }
    


template <class T>

static inline void DSTDigestis_empty(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,bool>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::tdigest<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.is_empty();
        });

}

    


template <class T>

static inline void DSTDigestk(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,uint16_t>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::tdigest<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_k();
        });

}

    


template <class T>

static inline void DSTDigestcdf(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 2);

    
        auto &sketch_vector = args.data[0];
        auto &split_points_vector = args.data[1];

    
        
                    UnifiedVectorFormat unified_split_points;
                    split_points_vector.ToUnifiedFormat(args.size(), unified_split_points);

                    // auto split_points_list_entries = UnifiedVectorFormat::GetData<list_entry_t>(unified_split_points);
                    //         auto split_points_validitiy = FlatVector::Validity(split_points_vector);

                    auto &split_points_list_children = ListVector::GetEntry(split_points_vector);

                    UnifiedVectorFormat split_points_children_unified;
                    split_points_list_children.ToUnifiedFormat(args.size(), split_points_children_unified);

                    const T *split_points_list_children_data = UnifiedVectorFormat::GetData<T>(split_points_children_unified);
                    
        BinaryExecutor::Execute
        <string_t,list_entry_t,list_entry_t>
        (
        sketch_vector,split_points_vector,result,args.size(),
        [&](string_t sketch_data,list_entry_t split_points_data) {

            auto sketch = datasketches::tdigest<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());

                    T *passing_points = (T *)duckdb_malloc(sizeof(T) * split_points_data.length);
                    for (idx_t i = 0; i < split_points_data.length; i++)
                    {
                        passing_points[i] = split_points_list_children_data[i + split_points_data.offset];
                    }
                    
auto cdf_result = sketch.get_CDF(passing_points, split_points_data.length);
                auto current_size = ListVector::GetListSize(result);
                auto new_size = current_size + cdf_result.size();
                if (ListVector::GetListCapacity(result) < new_size)
                {
                    ListVector::Reserve(result, new_size);
                }

                auto &child_entry = ListVector::GetEntry(result);
                auto child_vals = FlatVector::GetData<T>(child_entry);
                //auto &child_validity = FlatVector::Validity(child_entry);
                for (idx_t i = 0; i < cdf_result.size(); i++)
                {
                    child_vals[current_size + i] = cdf_result[i];
                }
                ListVector::SetListSize(result, new_size);
                return list_entry_t{current_size, cdf_result.size()};
                
        });

}

    


template <class T>

static inline void DSTDigestpmf(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 2);

    
        auto &sketch_vector = args.data[0];
        auto &split_points_vector = args.data[1];

    
        
                    UnifiedVectorFormat unified_split_points;
                    split_points_vector.ToUnifiedFormat(args.size(), unified_split_points);

                    // auto split_points_list_entries = UnifiedVectorFormat::GetData<list_entry_t>(unified_split_points);
                    //         auto split_points_validitiy = FlatVector::Validity(split_points_vector);

                    auto &split_points_list_children = ListVector::GetEntry(split_points_vector);

                    UnifiedVectorFormat split_points_children_unified;
                    split_points_list_children.ToUnifiedFormat(args.size(), split_points_children_unified);

                    const T *split_points_list_children_data = UnifiedVectorFormat::GetData<T>(split_points_children_unified);
                    
        BinaryExecutor::Execute
        <string_t,list_entry_t,list_entry_t>
        (
        sketch_vector,split_points_vector,result,args.size(),
        [&](string_t sketch_data,list_entry_t split_points_data) {

            auto sketch = datasketches::tdigest<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());

                    T *passing_points = (T *)duckdb_malloc(sizeof(T) * split_points_data.length);
                    for (idx_t i = 0; i < split_points_data.length; i++)
                    {
                        passing_points[i] = split_points_list_children_data[i + split_points_data.offset];
                    }
                    
auto pmf_result = sketch.get_PMF(passing_points, split_points_data.length);
                auto current_size = ListVector::GetListSize(result);
                auto new_size = current_size + pmf_result.size();
                if (ListVector::GetListCapacity(result) < new_size)
                {
                    ListVector::Reserve(result, new_size);
                }

                auto &child_entry = ListVector::GetEntry(result);
                auto child_vals = FlatVector::GetData<double>(child_entry);
                //auto &child_validity = FlatVector::Validity(child_entry);
                for (idx_t i = 0; i < pmf_result.size(); i++)
                {
                    child_vals[current_size + i] = pmf_result[i];
                }
                ListVector::SetListSize(result, new_size);
                return list_entry_t{current_size, pmf_result.size()};
                
        });

}

    


template <class T>

static inline void DSTDigestdescribe(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 2);

    
        auto &sketch_vector = args.data[0];
        auto &include_centroids_vector = args.data[1];

    
        
        BinaryExecutor::Execute
        <string_t,bool,string_t>
        (
        sketch_vector,include_centroids_vector,result,args.size(),
        [&](string_t sketch_data,bool include_centroids_data) {

            auto sketch = datasketches::tdigest<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return StringVector::AddString(result, sketch.to_string(include_centroids_data));
        });

}

    


template <class T>

static inline void DSTDigestrank(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 2);

    
        auto &sketch_vector = args.data[0];
        auto &item_vector = args.data[1];

    
        
        BinaryExecutor::Execute
        <string_t,T,double>
        (
        sketch_vector,item_vector,result,args.size(),
        [&](string_t sketch_data,T item_data) {

            auto sketch = datasketches::tdigest<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_rank(item_data);
        });

}

    


template <class T>

static inline void DSTDigesttotal_weight(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,uint64_t>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::tdigest<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_total_weight();
        });

}

    


template <class T>

static inline void DSTDigestquantile(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 2);

    
        auto &sketch_vector = args.data[0];
        auto &rank_vector = args.data[1];

    
        
        BinaryExecutor::Execute
        <string_t,double,T>
        (
        sketch_vector,rank_vector,result,args.size(),
        [&](string_t sketch_data,double rank_data) {

            auto sketch = datasketches::tdigest<T>::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_quantile(rank_data);
        });

}

    


    
    template <typename T>
    auto static DSTDigestMergeAggregate(const LogicalType &type, const LogicalType &result_type) -> AggregateFunction
    
    {
        
        return AggregateFunction::BinaryAggregate<DSTDigestState<T>, int32_t, string_t, string_t, DSQuantilesMergeOperation>(
            LogicalType::INTEGER, result_type, result_type);
        
    }



    template <typename T>
    auto static DSTDigestCreateAggregate(const LogicalType &type, const LogicalType &result_type) -> AggregateFunction
    {
        
        return AggregateFunction::BinaryAggregate<DSTDigestState<T>, int32_t, T, string_t, DSQuantilesCreateOperation>(
            LogicalType::INTEGER, type, result_type);
        
    }


  void LoadTDigestSketch(DatabaseInstance &instance) {
      auto &system_catalog = Catalog::GetSystemCatalog(instance);
      auto data = CatalogTransaction::GetSystemTransaction(instance);


      
        std::unordered_map<LogicalTypeId, LogicalType> sketch_map_types;
        
            sketch_map_types.insert({ LogicalType::FLOAT, CreateTDigestSketchType(instance, LogicalType(LogicalType::FLOAT))});
            sketch_map_types.insert({ LogicalType::DOUBLE, CreateTDigestSketchType(instance, LogicalType(LogicalType::DOUBLE))});
      


      
      {
        ScalarFunctionSet fs("datasketch_tdigest_is_empty");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::BOOLEAN
                ,    DSTDigestis_empty<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::BOOLEAN
                ,    DSTDigestis_empty<double>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_tdigest_k");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::USMALLINT
                ,    DSTDigestk<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::USMALLINT
                ,    DSTDigestk<double>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_tdigest_cdf");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::LIST(LogicalType::FLOAT)},LogicalType::LIST(LogicalType::FLOAT)
                ,    DSTDigestcdf<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::LIST(LogicalType::DOUBLE)},LogicalType::LIST(LogicalType::DOUBLE)
                ,    DSTDigestcdf<double>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_tdigest_pmf");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::LIST(LogicalType::FLOAT)},LogicalType::LIST(LogicalType::FLOAT)
                ,    DSTDigestpmf<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::LIST(LogicalType::DOUBLE)},LogicalType::LIST(LogicalType::DOUBLE)
                ,    DSTDigestpmf<double>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_tdigest_describe");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSTDigestdescribe<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSTDigestdescribe<double>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_tdigest_rank");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::FLOAT},LogicalType::DOUBLE
                ,    DSTDigestrank<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::DOUBLE},LogicalType::DOUBLE
                ,    DSTDigestrank<double>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_tdigest_total_weight");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT]},LogicalType::UBIGINT
                ,    DSTDigesttotal_weight<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE]},LogicalType::UBIGINT
                ,    DSTDigesttotal_weight<double>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_tdigest_quantile");
        
            
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::FLOAT],LogicalType::DOUBLE},LogicalType::FLOAT
                ,    DSTDigestquantile<float>));
                fs.AddFunction(ScalarFunction(
                {sketch_map_types[LogicalType::DOUBLE],LogicalType::DOUBLE},LogicalType::DOUBLE
                ,    DSTDigestquantile<double>));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }

      // This funciton creates the sketches.
      {
      AggregateFunctionSet sketch("datasketch_tdigest");
      
        
            sketch.AddFunction(DSTDigestCreateAggregate<float>(LogicalType::FLOAT, sketch_map_types[LogicalType::FLOAT]));
            sketch.AddFunction(DSTDigestMergeAggregate<float>(LogicalType::FLOAT, sketch_map_types[LogicalType::FLOAT]));
        
        
            sketch.AddFunction(DSTDigestCreateAggregate<double>(LogicalType::DOUBLE, sketch_map_types[LogicalType::DOUBLE]));
            sketch.AddFunction(DSTDigestMergeAggregate<double>(LogicalType::DOUBLE, sketch_map_types[LogicalType::DOUBLE]));
        
      CreateAggregateFunctionInfo sketch_info(sketch);
      system_catalog.CreateFunction(data, sketch_info);
      }



      


  }
    


static inline void DSHLLis_empty(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,bool>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::hll_sketch::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.is_empty();
        });

}

    


static inline void DSHLLdescribe(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 3);

    
        auto &sketch_vector = args.data[0];
        auto &summary_vector = args.data[1];
        auto &detail_vector = args.data[2];

    
        
        TernaryExecutor::Execute
        <string_t,bool,bool,string_t>
        (
        sketch_vector,summary_vector,detail_vector,result,args.size(),
        [&](string_t sketch_data,bool summary_data,bool detail_data) {

            auto sketch = datasketches::hll_sketch::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return StringVector::AddString(result, sketch.to_string(summary_data, detail_data, false, false));
        });

}

    


static inline void DSHLLlg_config_k(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,uint8_t>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::hll_sketch::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_lg_config_k();
        });

}

    


static inline void DSHLLis_compact(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,bool>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::hll_sketch::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.is_compact();
        });

}

    


static inline void DSHLLestimate(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,double>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::hll_sketch::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_estimate();
        });

}

    


static inline void DSHLLlower_bound(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 2);

    
        auto &sketch_vector = args.data[0];
        auto &std_dev_vector = args.data[1];

    
        
        BinaryExecutor::Execute
        <string_t,uint8_t,double>
        (
        sketch_vector,std_dev_vector,result,args.size(),
        [&](string_t sketch_data,uint8_t std_dev_data) {

            auto sketch = datasketches::hll_sketch::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_lower_bound(std_dev_data);
        });

}

    


static inline void DSHLLupper_bound(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 2);

    
        auto &sketch_vector = args.data[0];
        auto &std_dev_vector = args.data[1];

    
        
        BinaryExecutor::Execute
        <string_t,uint8_t,double>
        (
        sketch_vector,std_dev_vector,result,args.size(),
        [&](string_t sketch_data,uint8_t std_dev_data) {

            auto sketch = datasketches::hll_sketch::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_upper_bound(std_dev_data);
        });

}

    


    
    auto static DSHLLMergeAggregate(const LogicalType &result_type) -> AggregateFunction
    
    {
        
        return AggregateFunction::BinaryAggregate<DSHLLState, int32_t, string_t, string_t, DSHLLMergeOperation>(
            LogicalType::INTEGER, result_type, result_type);
        
    }



    template <typename T>
    auto static DSHLLCreateAggregate(const LogicalType &type, const LogicalType &result_type) -> AggregateFunction
    {
        
        return AggregateFunction::BinaryAggregate<DSHLLState, int32_t, T, string_t, DSHLLCreateOperation>(
            LogicalType::INTEGER, type, result_type);
        
    }


  void LoadHLLSketch(DatabaseInstance &instance) {
      auto &system_catalog = Catalog::GetSystemCatalog(instance);
      auto data = CatalogTransaction::GetSystemTransaction(instance);


      
        auto sketch_type = CreateHLLCountingSketchType(instance);
      


      
      {
        ScalarFunctionSet fs("datasketch_hll_is_empty");
        
                fs.AddFunction(ScalarFunction(
                {sketch_type},LogicalType::BOOLEAN
                ,    DSHLLis_empty));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_hll_describe");
        
                fs.AddFunction(ScalarFunction(
                {sketch_type,LogicalType::BOOLEAN,LogicalType::BOOLEAN},LogicalType::VARCHAR
                ,    DSHLLdescribe));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_hll_lg_config_k");
        
                fs.AddFunction(ScalarFunction(
                {sketch_type},LogicalType::UTINYINT
                ,    DSHLLlg_config_k));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_hll_is_compact");
        
                fs.AddFunction(ScalarFunction(
                {sketch_type},LogicalType::BOOLEAN
                ,    DSHLLis_compact));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_hll_estimate");
        
                fs.AddFunction(ScalarFunction(
                {sketch_type},LogicalType::DOUBLE
                ,    DSHLLestimate));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_hll_lower_bound");
        
                fs.AddFunction(ScalarFunction(
                {sketch_type,LogicalType::UTINYINT},LogicalType::DOUBLE
                ,    DSHLLlower_bound));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_hll_upper_bound");
        
                fs.AddFunction(ScalarFunction(
                {sketch_type,LogicalType::UTINYINT},LogicalType::DOUBLE
                ,    DSHLLupper_bound));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }

      // This funciton creates the sketches.
      {
      AggregateFunctionSet sketch("datasketch_hll");
      
        
            sketch.AddFunction(DSHLLCreateAggregate<int8_t>(LogicalType::TINYINT, sketch_type));
        
        
            sketch.AddFunction(DSHLLCreateAggregate<int16_t>(LogicalType::SMALLINT, sketch_type));
        
        
            sketch.AddFunction(DSHLLCreateAggregate<int32_t>(LogicalType::INTEGER, sketch_type));
        
        
            sketch.AddFunction(DSHLLCreateAggregate<int64_t>(LogicalType::BIGINT, sketch_type));
        
        
            sketch.AddFunction(DSHLLCreateAggregate<float>(LogicalType::FLOAT, sketch_type));
        
        
            sketch.AddFunction(DSHLLCreateAggregate<double>(LogicalType::DOUBLE, sketch_type));
        
        
            sketch.AddFunction(DSHLLCreateAggregate<uint8_t>(LogicalType::UTINYINT, sketch_type));
        
        
            sketch.AddFunction(DSHLLCreateAggregate<uint16_t>(LogicalType::USMALLINT, sketch_type));
        
        
            sketch.AddFunction(DSHLLCreateAggregate<uint32_t>(LogicalType::UINTEGER, sketch_type));
        
        
            sketch.AddFunction(DSHLLCreateAggregate<uint64_t>(LogicalType::UBIGINT, sketch_type));
        
        
            sketch.AddFunction(DSHLLCreateAggregate<string_t>(LogicalType::VARCHAR, sketch_type));
        
        
            sketch.AddFunction(DSHLLCreateAggregate<string_t>(LogicalType::BLOB, sketch_type));
        
      CreateAggregateFunctionInfo sketch_info(sketch);
      system_catalog.CreateFunction(data, sketch_info);
      }



      
      {
      AggregateFunctionSet sketch("datasketch_hll_union");
      sketch.AddFunction(DSHLLMergeAggregate(sketch_type));
      CreateAggregateFunctionInfo sketch_info(sketch);
      system_catalog.CreateFunction(data, sketch_info);
      }
      


  }
    


static inline void DSCPCis_empty(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,bool>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::cpc_sketch::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.is_empty();
        });

}

    


static inline void DSCPCdescribe(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,string_t>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::cpc_sketch::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return StringVector::AddString(result, sketch.to_string());
        });

}

    


static inline void DSCPCestimate(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 1);

    
        auto &sketch_vector = args.data[0];

    
        
        UnaryExecutor::Execute
        <string_t,double>
        (
        sketch_vector,result,args.size(),
        [&](string_t sketch_data) {

            auto sketch = datasketches::cpc_sketch::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_estimate();
        });

}

    


static inline void DSCPClower_bound(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 2);

    
        auto &sketch_vector = args.data[0];
        auto &std_dev_vector = args.data[1];

    
        
        BinaryExecutor::Execute
        <string_t,uint8_t,double>
        (
        sketch_vector,std_dev_vector,result,args.size(),
        [&](string_t sketch_data,uint8_t std_dev_data) {

            auto sketch = datasketches::cpc_sketch::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_lower_bound(std_dev_data);
        });

}

    


static inline void DSCPCupper_bound(DataChunk &args, ExpressionState &state, Vector &result)
{
    // Get the references to the incoming vectors.
    D_ASSERT(args.ColumnCount() == 2);

    
        auto &sketch_vector = args.data[0];
        auto &std_dev_vector = args.data[1];

    
        
        BinaryExecutor::Execute
        <string_t,uint8_t,double>
        (
        sketch_vector,std_dev_vector,result,args.size(),
        [&](string_t sketch_data,uint8_t std_dev_data) {

            auto sketch = datasketches::cpc_sketch::deserialize(sketch_data.GetDataUnsafe(), sketch_data.GetSize());
return sketch.get_upper_bound(std_dev_data);
        });

}

    


    
    auto static DSCPCMergeAggregate(const LogicalType &result_type) -> AggregateFunction
    
    {
        
        return AggregateFunction::BinaryAggregate<DSCPCState, int32_t, string_t, string_t, DSCPCMergeOperation>(
            LogicalType::INTEGER, result_type, result_type);
        
    }



    template <typename T>
    auto static DSCPCCreateAggregate(const LogicalType &type, const LogicalType &result_type) -> AggregateFunction
    {
        
        return AggregateFunction::BinaryAggregate<DSCPCState, int32_t, T, string_t, DSCPCCreateOperation>(
            LogicalType::INTEGER, type, result_type);
        
    }


  void LoadCPCSketch(DatabaseInstance &instance) {
      auto &system_catalog = Catalog::GetSystemCatalog(instance);
      auto data = CatalogTransaction::GetSystemTransaction(instance);


      
        auto sketch_type = CreateCPCCountingSketchType(instance);
      


      
      {
        ScalarFunctionSet fs("datasketch_cpc_is_empty");
        
                fs.AddFunction(ScalarFunction(
                {sketch_type},LogicalType::BOOLEAN
                ,    DSCPCis_empty));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_cpc_describe");
        
                fs.AddFunction(ScalarFunction(
                {sketch_type},LogicalType::VARCHAR
                ,    DSCPCdescribe));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_cpc_estimate");
        
                fs.AddFunction(ScalarFunction(
                {sketch_type},LogicalType::DOUBLE
                ,    DSCPCestimate));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_cpc_lower_bound");
        
                fs.AddFunction(ScalarFunction(
                {sketch_type,LogicalType::UTINYINT},LogicalType::DOUBLE
                ,    DSCPClower_bound));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }
      {
        ScalarFunctionSet fs("datasketch_cpc_upper_bound");
        
                fs.AddFunction(ScalarFunction(
                {sketch_type,LogicalType::UTINYINT},LogicalType::DOUBLE
                ,    DSCPCupper_bound));
        
        ExtensionUtil::RegisterFunction(instance, fs);
      }

      // This funciton creates the sketches.
      {
      AggregateFunctionSet sketch("datasketch_cpc");
      
        
            sketch.AddFunction(DSCPCCreateAggregate<int8_t>(LogicalType::TINYINT, sketch_type));
        
        
            sketch.AddFunction(DSCPCCreateAggregate<int16_t>(LogicalType::SMALLINT, sketch_type));
        
        
            sketch.AddFunction(DSCPCCreateAggregate<int32_t>(LogicalType::INTEGER, sketch_type));
        
        
            sketch.AddFunction(DSCPCCreateAggregate<int64_t>(LogicalType::BIGINT, sketch_type));
        
        
            sketch.AddFunction(DSCPCCreateAggregate<float>(LogicalType::FLOAT, sketch_type));
        
        
            sketch.AddFunction(DSCPCCreateAggregate<double>(LogicalType::DOUBLE, sketch_type));
        
        
            sketch.AddFunction(DSCPCCreateAggregate<uint8_t>(LogicalType::UTINYINT, sketch_type));
        
        
            sketch.AddFunction(DSCPCCreateAggregate<uint16_t>(LogicalType::USMALLINT, sketch_type));
        
        
            sketch.AddFunction(DSCPCCreateAggregate<uint32_t>(LogicalType::UINTEGER, sketch_type));
        
        
            sketch.AddFunction(DSCPCCreateAggregate<uint64_t>(LogicalType::UBIGINT, sketch_type));
        
        
            sketch.AddFunction(DSCPCCreateAggregate<string_t>(LogicalType::VARCHAR, sketch_type));
        
        
            sketch.AddFunction(DSCPCCreateAggregate<string_t>(LogicalType::BLOB, sketch_type));
        
      CreateAggregateFunctionInfo sketch_info(sketch);
      system_catalog.CreateFunction(data, sketch_info);
      }



      
      {
      AggregateFunctionSet sketch("datasketch_cpc_union");
      sketch.AddFunction(DSCPCMergeAggregate(sketch_type));
      CreateAggregateFunctionInfo sketch_info(sketch);
      system_catalog.CreateFunction(data, sketch_info);
      }
      


  }


}