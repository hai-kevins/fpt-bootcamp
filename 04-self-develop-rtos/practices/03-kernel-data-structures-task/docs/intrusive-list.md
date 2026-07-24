# Intrusive List

Node nằm trực tiếp trong object kernel. Cách này không cần wrapper allocation và phù hợp static-first. Một node chỉ thuộc một list; nhiều membership cần nhiều node. `container_of` chuyển node về object chứa nó.
