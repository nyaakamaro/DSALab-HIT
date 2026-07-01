# Hệ thống Quản lý Thư viện (kèm Thư viện số)

Ứng dụng console C++ quản lý thư viện: quản lý sách, độc giả, mượn/trả sách, lịch sử thao tác có Undo, thống kê, lưu/đọc file — và mở rộng thêm tính năng **thư viện số**: xem tóm tắt sách, đọc nội dung online, viết/xem bình luận và đánh giá sao.

## Cấu trúc file

| File | Nội dung |
|---|---|
| `library.h` | Khai báo các struct/class: `WaitQueue`, `CommentList`, `Book`, `Reader`, `HistoryStack`, `Library` |
| `library.cpp` | Cài đặt toàn bộ logic nghiệp vụ |
| `main.cpp` | Menu console, nhập liệu, và bộ test case tự động (demo nhanh) |

## Cấu trúc dữ liệu sử dụng

| Cấu trúc | Dùng cho | Vì sao |
|---|---|---|
| **Cây nhị phân tìm kiếm (BST)** | Lưu trữ sách (`Book`), khóa = mã sách | Tìm kiếm/thêm/xóa theo mã sách nhanh, duyệt inorder cho ra danh sách đã sắp xếp |
| **Danh sách liên kết đơn** | Danh sách độc giả (`Reader`) | Số lượng độc giả không cố định, chèn đầu O(1) |
| **Hàng đợi (Queue)** | Hàng chờ mượn sách (`WaitQueue`), **1 hàng đợi riêng cho mỗi cuốn sách** | Đảm bảo công bằng FIFO — ai đăng ký chờ trước được ưu tiên nhận sách trước khi có người trả |
| **Ngăn xếp (Stack)** | Lịch sử thao tác (`HistoryStack`) | Undo thao tác gần nhất trước — đúng bản chất LIFO |
| **Danh sách liên kết đơn** | Bình luận/đánh giá (`CommentList`), **1 danh sách riêng cho mỗi cuốn sách** | Số bình luận không giới hạn, thêm mới vào cuối để giữ đúng thứ tự thời gian |

## Các chức năng chính

### Quản lý sách & độc giả
1. Thêm / Xóa / Sửa sách
2. Tìm kiếm sách theo mã hoặc theo tên (không phân biệt hoa/thường)
3. Thêm độc giả, xem danh sách độc giả

### Mượn / Trả sách
4. Mượn sách — còn bản thì cho mượn ngay, hết bản thì tự động xếp vào hàng chờ riêng của cuốn sách đó
5. Trả sách — nếu có người đang chờ, sách được tự động giao ngay cho người đầu hàng đợi (FIFO)

### Lịch sử & Undo
6. Xem lịch sử thao tác (mới nhất trước)
7. Undo thao tác gần nhất — hoàn tác đúng loại thao tác (thêm/xóa/sửa sách, mượn/trả sách)

### Thống kê
8. Sách được mượn nhiều nhất
9. Độc giả tích cực nhất (tổng lượt mượn)

### Lưu / đọc file
10. Lưu toàn bộ dữ liệu (sách, độc giả, bình luận) ra file `.txt`
11. Đọc lại dữ liệu từ file — tương thích ngược với file cũ (không có tóm tắt/bình luận vẫn đọc được)

### 📚 Thư viện số (tính năng mới)
12. **Xem tóm tắt sách** — mỗi sách có thể có phần tóm tắt nội dung
13. **Đọc nội dung online** — mỗi sách có trường `noi_dung_online`:
    - Nếu bắt đầu bằng `http://` hoặc `https://` → được coi là **link đọc online**, chương trình in ra link (và có thể tự mở bằng trình duyệt mặc định nếu chạy trên máy có giao diện đồ họa)
    - Nếu là văn bản thường → được coi là **nội dung đầy đủ**, in trực tiếp ra màn hình
14. **Cập nhật tóm tắt / link đọc online** cho sách đã tồn tại
15. **Viết bình luận + đánh giá sao (1-5)** cho từng sách
16. **Xem toàn bộ bình luận** của một cuốn sách kèm **điểm đánh giá trung bình**
17. **Xem chi tiết sách** — màn hình tổng hợp: thông tin cơ bản + tóm tắt + link online + điểm đánh giá

## Định dạng file lưu trữ

File lưu theo 3 khối, phân tách bằng dấu `|` (sách/độc giả) hoặc `~` (bình luận, để tránh nhầm với `|` có thể xuất hiện trong nội dung):

```
#SACH
ma_sach|ten_sach|tac_gia|the_loai|so_luong|so_luong_con|luot_muon|tom_tat|noi_dung_online

#DOCGIA
ma_doc_gia|ho_ten|sdt|so_sach_dang_muon|tong_luot_muon

#BINHLUAN
ma_sach~ma_doc_gia~ho_ten~so_sao~thoi_gian~noi_dung
```

## Menu chương trình

```
 1. Them sach                              12. Thong ke doc gia tich cuc nhat
 2. Xoa sach                                13. In toan bo danh sach sach
 3. Sua thong tin sach                      14. Luu du lieu ra file
 4. Tim kiem sach (theo ma hoac ten)        15. Doc du lieu tu file
 5. Them doc gia                            16. Chay bo test case tu dong (demo nhanh)
 6. Xem danh sach doc gia               ---------------- THU VIEN SO ----------------
 7. Muon sach                               17. Xem chi tiet sach (tom tat + link + diem danh gia)
 8. Tra sach                                18. Xem tom tat sach
 9. Xem lich su thao tac                    19. Doc noi dung online
10. Undo thao tac cuoi                      20. Cap nhat tom tat / link doc online cho sach
11. Thong ke sach muon nhieu nhat           21. Viet binh luan / danh gia sach
                                             22. Xem binh luan / danh gia sach
                                              0. Thoat
```

## Biên dịch & chạy

Yêu cầu trình biên dịch hỗ trợ C++17.

```bash
g++ -std=c++17 -Wall -Wextra -o thuvien main.cpp library.cpp
./thuvien
```

Trên Windows (ví dụ dùng MinGW):

```bash
g++ -std=c++17 -o thuvien.exe main.cpp library.cpp
thuvien.exe
```

> Ghi chú: hàm lấy thời gian hiện tại (`getCurrentTime`) đã được viết tương thích cả Windows (`localtime_s`) lẫn Linux/Mac (`localtime_r`) nên có thể biên dịch trên mọi hệ điều hành.

Chọn mục **16** trong menu để chạy nhanh bộ test case tự động — demo toàn bộ 22 chức năng chỉ trong vài giây, bao gồm cả tóm tắt, đọc online, và bình luận/đánh giá.

## Hướng mở rộng thêm (gợi ý)

- Đọc nội dung sách từ **file riêng** (ví dụ `.txt`/`.pdf` từng chương) thay vì nhét toàn bộ text vào 1 dòng trong file lưu trữ
- Cho phép **sửa/xóa bình luận** của chính độc giả đã viết
- Thêm **phân trang** khi in danh sách sách/bình luận nếu dữ liệu lớn
- Xây **giao diện web/desktop** dùng lại toàn bộ lớp `Library` làm phần lõi (backend logic không đổi)
