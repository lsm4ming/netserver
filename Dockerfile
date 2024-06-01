FROM ubuntu:24.04

# 替换apt源为阿里云镜像
RUN sed -i 's|http://archive.ubuntu.com/ubuntu/|http://mirrors.aliyun.com/ubuntu/|g' /etc/apt/sources.list

# 更新软件包列表并安装基本工具
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    curl \
    wget \
    git \
    sudo \
    vim \
    lsb-release \
    software-properties-common \
    tree \
    python3 \
    python3-pip

# 安装C++编译器
RUN apt-get install -y g++ gcc make cmake gdb

# 安装conan
# RUN pip install conan --break-system-packages
# 安装Conan 2
RUN pip3 install conan==2.* --break-system-packages --ignore-installed

# 安装nodejs
RUN apt-get install nodejs -y

# 安装Rust工具链
ENV RUSTUP_DIST_SERVER=https://mirrors.ustc.edu.cn/rust-static
ENV RUSTUP_UPDATE_ROOT=https://mirrors.ustc.edu.cn/rust-static/rustup
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
ENV PATH=/root/.cargo/bin:$PATH

# 清理不必要的包缓存
RUN apt-get clean && rm -rf /var/lib/apt/lists/*

# 创建一个非root用户并设置密码
RUN useradd -ms /bin/bash dev && echo "dev:dev" | chpasswd && adduser dev sudo

# 切换到非root用户
USER dev
WORKDIR /home/dev

# 导出环境变量
ENV PATH=/home/dev/.cargo/bin:$PATH

# 启动bash
CMD ["bash"]
