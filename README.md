#### Features
- set locale to "en_US.UTF-8", 避免乱码
- use UTF-8 encoding
- 搜索命令历史 前缀匹配
- shortcut
  - tree
  - ll
- 预装常用工具
  - vim-tiny, 用vi/vim 命令即可使用
  - ps
  - tree
- better apt-get
  可以根据环境变量 APT_SOURCE 中指定的源列表, 依次重试，直到 apt-get 执行成功，避免单个 apt 源失效的问题. 可用的源有:
  - debian/official/default
  - aliyun
  - 163
  如，APT_SOURCE="aliyun,163,default", 就是说执行apt-get 时，先尝试 aliyun 的镜像源，如果失败，就尝试 163 的镜像源，最后再尝试 debian 官方的源.
