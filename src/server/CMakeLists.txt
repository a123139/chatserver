# 把.指定路径下的所有源文件名字放入变量名SRC_LIST里面
aux_source_directory(. SRC_LIST)
aux_source_directory(./db DB_LIST)
aux_source_directory(./model MODEL_LIST)
aux_source_directory(./redis REDIS_LIST)
# 生成可执行文件 server
add_executable(Chatserver ${SRC_LIST} ${DB_LIST} ${MODEL_LIST} ${REDIS_LIST})
# 链接 muduo 库 + 线程库（必须！）
target_link_libraries(Chatserver  muduo_net  muduo_base pthread mysqlclient hiredis)

