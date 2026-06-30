# Hệ thống Quản lý Thư Viện (C++ CLI)

Ứng dụng quản lý sách, độc giả và lịch sử mượn/trả, chạy trên dòng lệnh (CLI).

## Cấu trúc dữ liệu sử dụng

| Cấu trúc | Vai trò |
|---|---|
| BST | Lưu trữ sách, tìm kiếm theo mã/tên — O(log n) |
| Queue | Danh sách chờ mượn khi sách hết (mỗi cuốn sách có 1 hàng đợi riêng) |
| Stack | Lịch sử thao tác — hỗ trợ Undo |
| Linked List | Danh sách độc giả |

Tất cả Queue/Stack/Linked List đều được tự cài đặt thủ công (không dùng `std::queue`/`std::stack`).

## Cấu trúc file

```
library_app/
├── library.h      # Khai báo struct (Book, Reader, QueueNode, HistoryAction) và class Library
├── library.cpp    # Cài đặt toàn bộ logic (BST, Queue, Stack, 7 chức năng)
└── main.cpp       # Menu CLI + bộ test case tự động
```

## Cách biên dịch và chạy

```bash
g++ -std=c++17 -o thuvien main.cpp library.cpp
./thuvien
```

Trên Windows (MinGW): `g++ -std=c++17 -o thuvien.exe main.cpp library.cpp` rồi chạy `thuvien.exe`.

## Cách dùng nhanh

Sau khi chạy, chọn số chức năng trong menu hiện ra. Để xem demo toàn bộ 7 chức năng ngay lập tức với dữ liệu mẫu (không cần gõ tay), chọn:

```
16. Chay bo test case tu dong (demo nhanh)
```

## Danh sách chức năng (menu)

1. Thêm sách
2. Xóa sách
3. Sửa thông tin sách
4. Tìm kiếm sách (theo mã hoặc tên — dùng BST)
5. Thêm độc giả
6. Xem danh sách độc giả
7. Mượn sách (tự động vào hàng chờ nếu sách hết)
8. Trả sách (tự động giao cho người chờ tiếp theo nếu có)
9. Xem lịch sử thao tác (Stack)
10. Undo thao tác cuối
11. Thống kê sách được mượn nhiều nhất
12. Thống kê độc giả tích cực nhất
13. In toàn bộ danh sách sách (duyệt BST in-order → tự động sắp xếp theo mã)
14. Lưu dữ liệu ra file .txt
15. Đọc dữ liệu từ file .txt
16. Chạy bộ test case tự động (demo nhanh)
0. Thoát

## Định dạng file dữ liệu (.txt)

```
#SACH
ma|ten|tac_gia|the_loai|so_luong|so_luong_con|luot_muon
#DOCGIA
ma|ho_ten|sdt|so_sach_dang_muon|tong_luot_muon
```

## Lưu ý

- Undo chỉ hoàn tác **1 bước gần nhất** mỗi lần gọi (chưa hỗ trợ Redo).
- Nếu sửa sách làm giảm tổng số lượng xuống thấp hơn số đang được mượn, hệ thống tự kẹp số bản còn lại về 0 thay vì báo lỗi.
- Mã sách (`ma_sach`) là khóa duy nhất trên cây BST — không cho phép trùng.
