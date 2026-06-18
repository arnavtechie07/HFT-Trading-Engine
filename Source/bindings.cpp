#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../include/Order.h"
#include "../include/OrderBook.h"

namespace py = pybind11;

PYBIND11_MODULE(engine, m) {
    m.doc() = "Ultra-Low Latency Matching Engine written in C++";

    //Defining the Order Struct to Python
    py::class_<Order>(m, "Order")
        //Order struct constructor for Python 
        .def(py::init<uint64_t, bool, uint64_t, uint64_t>())
        .def_readwrite("order_id", &Order::orderId)
        .def_readwrite("is_buy", &Order::isBuy)
        .def_readwrite("quantity", &Order::quantity)
        .def_readwrite("price", &Order::price);

    //OrderBook class constructor for Python
    py::class_<OrderBook>(m, "OrderBook")
        .def(py::init<>()) 
        .def("add_order", &OrderBook::addOrder)
        .def("cancel_order", &OrderBook::cancelOrder)
        .def("print_book", &OrderBook::printBook)
        .def("get_bids", &OrderBook::getBids)
        .def("get_asks", &OrderBook::getAsks);
}