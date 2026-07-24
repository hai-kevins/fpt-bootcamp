# Ready Queues

Một FIFO intrusive list cho mỗi priority. Task READY nằm đúng một queue bằng `ready_node`. Ready bitmap phản ánh queue không rỗng. Scheduler chọn head của priority cao nhất; rotation đưa head về tail cho round-robin.
