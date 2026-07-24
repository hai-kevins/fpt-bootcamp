# Priority Inheritance

Khi High block trên mutex do Low giữ, Low được boost effective priority. Scheduler phải di chuyển READY owner sang ready queue mới. Sau unlock hoặc waiter timeout, effective priority được tính lại từ base priority và các mutex còn sở hữu.

Transitive inheritance nhiều tầng chưa thuộc phạm vi project này.
