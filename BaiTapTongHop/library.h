#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <vector>

// ============ QUEUE: danh sách chờ mượn sách (1 hàng đợi cho mỗi cuốn sách) ============
struct QueueNode {
    std::string ma_doc_gia;
    QueueNode* next;
    QueueNode(const std::string& ma) : ma_doc_gia(ma), next(nullptr) {}
};

class WaitQueue {
private:
    QueueNode* front;
    QueueNode* rear;
    int count;
public:
    WaitQueue();
    ~WaitQueue();
    void push(const std::string& ma_doc_gia);
    std::string pop();          // lấy người đầu hàng đợi ra (FIFO)
    bool isEmpty() const;
    int size() const;
    std::vector<std::string> toVector() const; // để in/lưu file
};

// ============ BST: lưu trữ sách, khóa = mã sách ============
struct Book {
    std::string ma_sach;
    std::string ten_sach;
    std::string tac_gia;
    std::string the_loai;
    int so_luong;        // tổng số bản
    int so_luong_con;    // số bản còn lại
    int luot_muon;       // tổng lượt mượn (thống kê)

    WaitQueue hang_cho;  // queue chờ mượn riêng cho sách này

    Book* left;
    Book* right;

    Book(const std::string& ma, const std::string& ten, const std::string& tg,
         const std::string& tl, int sl)
        : ma_sach(ma), ten_sach(ten), tac_gia(tg), the_loai(tl),
          so_luong(sl), so_luong_con(sl), luot_muon(0),
          left(nullptr), right(nullptr) {}
};

// ============ LINKED LIST: danh sách độc giả ============
struct Reader {
    std::string ma_doc_gia;
    std::string ho_ten;
    std::string sdt;
    int so_sach_dang_muon;
    int tong_luot_muon;   // thống kê độc giả tích cực nhất
    Reader* next;

    Reader(const std::string& ma, const std::string& ten, const std::string& sdt_)
        : ma_doc_gia(ma), ho_ten(ten), sdt(sdt_),
          so_sach_dang_muon(0), tong_luot_muon(0), next(nullptr) {}
};

// ============ STACK: lịch sử thao tác, hỗ trợ Undo ============
enum ActionType { THEM_SACH, XOA_SACH, SUA_SACH, MUON_SACH, TRA_SACH };

struct HistoryAction {
    ActionType type;
    std::string ma_sach;
    std::string ma_doc_gia;
    // dữ liệu cũ để phục vụ Undo (lưu dạng chuỗi mô tả ngắn gọn)
    std::string old_ten, old_tac_gia, old_the_loai;
    int old_so_luong;
    std::string thoi_gian;

    HistoryAction* next;

    HistoryAction(ActionType t, const std::string& ms, const std::string& md,
                  const std::string& tg)
        : type(t), ma_sach(ms), ma_doc_gia(md),
          old_so_luong(0), thoi_gian(tg), next(nullptr) {}
};

class HistoryStack {
private:
    HistoryAction* top;
    int count;
public:
    HistoryStack();
    ~HistoryStack();
    void push(HistoryAction* action);
    HistoryAction* pop();   // lấy ra thao tác gần nhất (không xóa, trả về để xử lý Undo)
    HistoryAction* peek() const;
    bool isEmpty() const;
    int size() const;
    std::vector<HistoryAction*> toVector() const; // để duyệt in lịch sử (mới nhất trước)
};

// ============ LIBRARY: lớp quản lý tổng, gom toàn bộ chức năng ============
class Library {
private:
    Book* root;            // gốc BST sách
    Reader* readerHead;    // đầu danh sách liên kết độc giả
    HistoryStack history;  // stack lịch sử thao tác

    // BST helpers
    Book* insertBST(Book* node, Book* newBook);
    Book* searchBST(Book* node, const std::string& ma_sach) const;
    Book* findMinNode(Book* node) const;
    Book* deleteBST(Book* node, const std::string& ma_sach);
    void inorderCollect(Book* node, std::vector<Book*>& result) const;
    void destroyBST(Book* node);
    void searchByNameHelper(Book* node, const std::string& ten, std::vector<Book*>& result) const;

    // Reader helpers
    Reader* findReader(const std::string& ma_doc_gia) const;

    std::string getCurrentTime() const;

public:
    Library();
    ~Library();

    // 1. Thêm / Xóa / Sửa sách
    bool themSach(const std::string& ma, const std::string& ten, const std::string& tg,
                  const std::string& tl, int sl);
    bool xoaSach(const std::string& ma);
    bool suaSach(const std::string& ma, const std::string& ten_moi, const std::string& tg_moi,
                 const std::string& tl_moi, int sl_moi);

    // 2. Tìm kiếm
    Book* timTheoMa(const std::string& ma) const;
    std::vector<Book*> timTheoTen(const std::string& ten) const;

    // Quản lý độc giả
    bool themDocGia(const std::string& ma, const std::string& ten, const std::string& sdt);
    void inDanhSachDocGia() const;

    // 3 & 4. Mượn / Trả sách
    bool muonSach(const std::string& ma_sach, const std::string& ma_doc_gia);
    bool traSach(const std::string& ma_sach, const std::string& ma_doc_gia);

    // 5. Lịch sử & Undo
    void xemLichSu() const;
    bool undoLast();

    // 6. Thống kê
    void thongKeSachMuonNhieuNhat() const;
    void thongKeDocGiaTichCucNhat() const;

    // 7. Lưu / đọc file
    bool luuFile(const std::string& filename) const;
    bool docFile(const std::string& filename);

    // Hỗ trợ in toàn bộ
    void inToanBoSach() const;
};

#endif
