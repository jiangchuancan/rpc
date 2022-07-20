set(SRC_LIST 
    mprpcapplication.cc 
    mprpcconfig.cc 
    rpcprovider.cc
    rpcheader.pb.cc
    mprpcchannel.cc
    mprpccontroller.cc
    logger.cc
    zookeeperutil.cc)

#生成mprpc静态库
add_library(mprpc ${SRC_LIST})

target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt.a)