#include <winrt/base.h>

WINRT_EXPORT namespace std 
{
    template<> struct hash<winrt::Windows::Foundation::IUnknown> : winrt::impl::hash_base<winrt::Windows::Foundation::IUnknown> {};
}

#include "pybase.h"

PyTypeObject* py::winrt_type<py::winrt_base>::python_type;

PyDoc_STRVAR(winrt_base_doc, "base class for wrapped WinRT object instances.");

static PyType_Slot winrt_base_Type_slots[] =
{
    {Py_tp_doc, winrt_base_doc},
    { 0, nullptr },
};

static PyType_Spec winrt_base_Type_spec =
{
    "_winrt_base",
    0,
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    winrt_base_Type_slots
};

std::unordered_map<std::size_t, PyObject*> instance_map{};

void py::wrapped_instance(std::size_t key, PyObject* obj)
{
    // TODO: re-enable instance wrapper caching 

    // if obj is null, remove from instance_map
    //if (obj)
    //{
    //    auto insert = instance_map.try_emplace(key, obj);

    //    if (insert.second == false)
    //    {
    //        throw winrt::hresult_invalid_argument(L"wrapped WinRT object already cached");
    //    }
    //}
    //else
    //{
    //    // TODO: clean up the wrapped WinRT object. Currently leaking
    //    instance_map.extract(key);
    //}
}

PyObject* py::wrapped_instance(std::size_t key)
{
    //auto const it = instance_map.find(key);
    //if (it == instance_map.end())
    {
        return nullptr;
    }

    //return it->second;
}

static PyObject* init_apartment(PyObject* /*unused*/, PyObject* /*unused*/)
{
    winrt::init_apartment();
    Py_RETURN_NONE;
}

static PyObject* uninit_apartment(PyObject* /*unused*/, PyObject* /*unused*/)
{
    winrt::uninit_apartment();
    Py_RETURN_NONE;
}

static PyMethodDef module_methods[]{
    { "init_apartment", init_apartment, METH_NOARGS, "initialize the apartment" },
    { "uninit_apartment", uninit_apartment, METH_NOARGS, "uninitialize the apartment" },
    { nullptr }
};

static int module_exec(PyObject* module)
{
    PyObject* type_object = PyType_FromSpec(&winrt_base_Type_spec);
    if (type_object == nullptr)
    {
        return -1;
    }
    if (PyModule_AddObject(module, "_winrt_base", type_object) != 0)
    {
        Py_DECREF(type_object);
        return -1;
    }
    py::winrt_type<py::winrt_base>::python_type = reinterpret_cast<PyTypeObject*>(type_object);
    type_object = nullptr;

    return 0;
}
