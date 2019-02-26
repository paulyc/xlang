﻿// WARNING: Please don't edit this file. It was generated by Python/WinRT

#pragma once

#include "pybase.h"

#if __has_include("py.Windows.Data.Text.h")
#include "py.Windows.Data.Text.h"
#endif

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.Storage.h")
#include "py.Windows.Storage.h"
#endif

#if __has_include("py.Windows.Storage.FileProperties.h")
#include "py.Windows.Storage.FileProperties.h"
#endif

#if __has_include("py.Windows.Storage.Streams.h")
#include "py.Windows.Storage.Streams.h"
#endif

#include <winrt/Windows.Storage.Search.h>

namespace py
{
    template<>
    struct winrt_type<winrt::Windows::Storage::Search::ContentIndexer>
    {
        static PyTypeObject* python_type;
    
        static PyTypeObject* get_python_type()
        {
            return python_type;
        }
    };
    
    template<>
    struct winrt_type<winrt::Windows::Storage::Search::ContentIndexerQuery>
    {
        static PyTypeObject* python_type;
    
        static PyTypeObject* get_python_type()
        {
            return python_type;
        }
    };
    
    template<>
    struct winrt_type<winrt::Windows::Storage::Search::IndexableContent>
    {
        static PyTypeObject* python_type;
    
        static PyTypeObject* get_python_type()
        {
            return python_type;
        }
    };
    
    template<>
    struct winrt_type<winrt::Windows::Storage::Search::QueryOptions>
    {
        static PyTypeObject* python_type;
    
        static PyTypeObject* get_python_type()
        {
            return python_type;
        }
    };
    
    template<>
    struct winrt_type<winrt::Windows::Storage::Search::SortEntryVector>
    {
        static PyTypeObject* python_type;
    
        static PyTypeObject* get_python_type()
        {
            return python_type;
        }
    };
    
    template<>
    struct winrt_type<winrt::Windows::Storage::Search::StorageFileQueryResult>
    {
        static PyTypeObject* python_type;
    
        static PyTypeObject* get_python_type()
        {
            return python_type;
        }
    };
    
    template<>
    struct winrt_type<winrt::Windows::Storage::Search::StorageFolderQueryResult>
    {
        static PyTypeObject* python_type;
    
        static PyTypeObject* get_python_type()
        {
            return python_type;
        }
    };
    
    template<>
    struct winrt_type<winrt::Windows::Storage::Search::StorageItemQueryResult>
    {
        static PyTypeObject* python_type;
    
        static PyTypeObject* get_python_type()
        {
            return python_type;
        }
    };
    
    template<>
    struct winrt_type<winrt::Windows::Storage::Search::StorageLibraryChangeTrackerTriggerDetails>
    {
        static PyTypeObject* python_type;
    
        static PyTypeObject* get_python_type()
        {
            return python_type;
        }
    };
    
    template<>
    struct winrt_type<winrt::Windows::Storage::Search::StorageLibraryContentChangedTriggerDetails>
    {
        static PyTypeObject* python_type;
    
        static PyTypeObject* get_python_type()
        {
            return python_type;
        }
    };
    
    template<>
    struct winrt_type<winrt::Windows::Storage::Search::ValueAndLanguage>
    {
        static PyTypeObject* python_type;
    
        static PyTypeObject* get_python_type()
        {
            return python_type;
        }
    };
    
    template<>
    struct winrt_type<winrt::Windows::Storage::Search::IIndexableContent>
    {
        static PyTypeObject* python_type;
    
        static PyTypeObject* get_python_type()
        {
            return python_type;
        }
    };
    
    template<>
    struct winrt_type<winrt::Windows::Storage::Search::IStorageFolderQueryOperations>
    {
        static PyTypeObject* python_type;
    
        static PyTypeObject* get_python_type()
        {
            return python_type;
        }
    };
    
    template<>
    struct winrt_type<winrt::Windows::Storage::Search::IStorageQueryResultBase>
    {
        static PyTypeObject* python_type;
    
        static PyTypeObject* get_python_type()
        {
            return python_type;
        }
    };
    
    template<>
    struct winrt_type<winrt::Windows::Storage::Search::SortEntry>
    {
        static PyTypeObject* python_type;
    
        static PyTypeObject* get_python_type()
        {
            return python_type;
        }
    };
    
    template<>
    struct converter<winrt::Windows::Storage::Search::SortEntry>
    {
        static PyObject* convert(winrt::Windows::Storage::Search::SortEntry instance) noexcept;
        static winrt::Windows::Storage::Search::SortEntry convert_to(PyObject* obj);
    };
    
}
