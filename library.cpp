#include "library.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <cstdlib>

// ================= WaitQueue =================
WaitQueue::WaitQueue() : front(nullptr), rear(nullptr), count(0) {}

WaitQueue::~WaitQueue() {
    while (!isEmpty()) pop();
}

void WaitQueue::push(const std::string& ma_doc_gia) {
    QueueNode* node = new QueueNode(ma_doc_gia);
    if (rear == nullptr) {
        front = rear = node;
    } else {
        rear->next = node;
        rear = node;
    }
    count++;
}

std::string WaitQueue::pop() {
    if (isEmpty()) return "";
    QueueNode* temp = front;
    std::string ma = temp->ma_doc_gia;
    front = front->next;
    if (front == nullptr) rear = nullptr;
    delete temp;
    count--;
    return ma;
}

bool WaitQueue::isEmpty() const { return front == nullptr; }
int WaitQueue::size() const { return count; }

std::vector<std::string> WaitQueue::toVector() const {
    std::vector<std::string> result;
    QueueNode* cur = front;
    while (cur != nullptr) {
        result.push_back(cur->ma_doc_gia);
        cur = cur->next;
    }
    return result;
}

// ================= CommentList =================
CommentList::CommentList() : head(nullptr), tail(nullptr), count(0) {}

CommentList::~CommentList() {
    CommentNode* cur = head;
    while (cur != nullptr) {
        CommentNode* temp = cur;
        cur = cur->next;
        delete temp;
    }
}

void CommentList::push(const std::string& ma_doc_gia, const std::string& ho_ten,
                        const std::string& noi_dung, int so_sao, const std::string& thoi_gian) {
    int sao = so_sao;
    if (sao < 1) sao = 1;
    if (sao > 5) sao = 5;
    CommentNode* node = new CommentNode(ma_doc_gia, ho_ten, noi_dung, sao, thoi_gian);
    if (tail == nullptr) {
        head = tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
    count++;
}

bool CommentList::isEmpty() const { return head == nullptr; }
int CommentList::size() const { return count; }

double CommentList::diemTrungBinh() const {
    if (head == nullptr) return 0.0;
    int tong = 0;
    CommentNode* cur = head;
    while (cur != nullptr) {
        tong += cur->so_sao;
        cur = cur->next;
    }
    return static_cast<double>(tong) / count;
}

std::vector<CommentNode*> CommentList::toVector() const {
    std::vector<CommentNode*> result;
    CommentNode* cur = head;
    while (cur != nullptr) {
        result.push_back(cur);
        cur = cur->next;
    }
    return result;
}

// ================= HistoryStack =================
HistoryStack::HistoryStack() : top(nullptr), count(0) {}

HistoryStack::~HistoryStack() {
    while (!isEmpty()) {
        HistoryAction* a = pop();
        delete a;
    }
}

void HistoryStack::push(HistoryAction* action) {
    action->next = top;
    top = action;
    count++;
}

HistoryAction* HistoryStack::pop() {
    if (isEmpty()) return nullptr;
    HistoryAction* temp = top;
    top = top->next;
    count--;
    temp->next = nullptr;
    return temp;
}

HistoryAction* HistoryStack::peek() const { return top; }
bool HistoryStack::isEmpty() const { return top == nullptr; }
int HistoryStack::size() const { return count; }

std::vector<HistoryAction*> HistoryStack::toVector() const {
    std::vector<HistoryAction*> result;
    HistoryAction* cur = top;
    while (cur != nullptr) {
        result.push_back(cur);
        cur = cur->next;
    }
    return result;
}

// ================= Library =================
Library::Library() : root(nullptr), readerHead(nullptr) {}

Library::~Library() {
    destroyBST(root);
    Reader* cur = readerHead;
    while (cur != nullptr) {
        Reader* temp = cur;
        cur = cur->next;
        delete temp;
    }
}

std::string Library::getCurrentTime() const {
    time_t now = time(nullptr);
    tm lt;
#if defined(_WIN32)
    localtime_s(&lt, &now);
#else
    localtime_r(&now, &lt);
#endif
    char buf[64];
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", &lt);
    return std::string(buf);
}

// ---------- BST helpers ----------
Book* Library::insertBST(Book* node, Book* newBook) {
    if (node == nullptr) return newBook;
    if (newBook->ma_sach < node->ma_sach)
        node->left = insertBST(node->left, newBook);
    else if (newBook->ma_sach > node->ma_sach)
        node->right = insertBST(node->right, newBook);
    return node; // trùng mã: không chèn (đã kiểm tra trước khi gọi)
}

Book* Library::searchBST(Book* node, const std::string& ma_sach) const {
    if (node == nullptr || node->ma_sach == ma_sach) return node;
    if (ma_sach < node->ma_sach) return searchBST(node->left, ma_sach);
    return searchBST(node->right, ma_sach);
}

Book* Library::findMinNode(Book* node) const {
    while (node->left != nullptr) node = node->left;
    return node;
}

Book* Library::deleteBST(Book* node, const std::string& ma_sach) {
    if (node == nullptr) return nullptr;
    if (ma_sach < node->ma_sach) {
        node->left = deleteBST(node->left, ma_sach);
    } else if (ma_sach > node->ma_sach) {
        node->right = deleteBST(node->right, ma_sach);
    } else {
        // node cần xóa
        if (node->left == nullptr) {
            Book* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            Book* temp = node->left;
            delete node;
            return temp;
        }
        Book* successor = findMinNode(node->right);
        // copy dữ liệu (không copy con trỏ cây / queue)
        node->ma_sach = successor->ma_sach;
        node->ten_sach = successor->ten_sach;
        node->tac_gia = successor->tac_gia;
        node->the_loai = successor->the_loai;
        node->so_luong = successor->so_luong;
        node->so_luong_con = successor->so_luong_con;
        node->luot_muon = successor->luot_muon;
        node->right = deleteBST(node->right, successor->ma_sach);
    }
    return node;
}

void Library::inorderCollect(Book* node, std::vector<Book*>& result) const {
    if (node == nullptr) return;
    inorderCollect(node->left, result);
    result.push_back(node);
    inorderCollect(node->right, result);
}

void Library::destroyBST(Book* node) {
    if (node == nullptr) return;
    destroyBST(node->left);
    destroyBST(node->right);
    delete node;
}

void Library::searchByNameHelper(Book* node, const std::string& ten, std::vector<Book*>& result) const {
    if (node == nullptr) return;
    searchByNameHelper(node->left, ten, result);
    std::string a = node->ten_sach, b = ten;
    std::transform(a.begin(), a.end(), a.begin(), ::tolower);
    std::transform(b.begin(), b.end(), b.begin(), ::tolower);
    if (a.find(b) != std::string::npos) result.push_back(node);
    searchByNameHelper(node->right, ten, result);
}

// ---------- Reader helpers ----------
Reader* Library::findReader(const std::string& ma_doc_gia) const {
    Reader* cur = readerHead;
    while (cur != nullptr) {
        if (cur->ma_doc_gia == ma_doc_gia) return cur;
        cur = cur->next;
    }
    return nullptr;
}

// ---------- 1. Thêm / Xóa / Sửa sách ----------
bool Library::themSach(const std::string& ma, const std::string& ten, const std::string& tg,
                        const std::string& tl, int sl,
                        const std::string& tom_tat, const std::string& noi_dung_online) {
    if (searchBST(root, ma) != nullptr) {
        std::cout << "[Loi] Ma sach '" << ma << "' da ton tai!\n";
        return false;
    }
    Book* newBook = new Book(ma, ten, tg, tl, sl, tom_tat, noi_dung_online);
    root = insertBST(root, newBook);

    HistoryAction* act = new HistoryAction(THEM_SACH, ma, "", getCurrentTime());
    history.push(act);

    std::cout << "[OK] Da them sach: " << ten << " (" << ma << ")\n";
    return true;
}

bool Library::xoaSach(const std::string& ma) {
    Book* b = searchBST(root, ma);
    if (b == nullptr) {
        std::cout << "[Loi] Khong tim thay sach ma " << ma << "\n";
        return false;
    }
    HistoryAction* act = new HistoryAction(XOA_SACH, ma, "", getCurrentTime());
    act->old_ten = b->ten_sach;
    act->old_tac_gia = b->tac_gia;
    act->old_the_loai = b->the_loai;
    act->old_so_luong = b->so_luong;
    history.push(act);

    root = deleteBST(root, ma);
    std::cout << "[OK] Da xoa sach ma " << ma << "\n";
    return true;
}

bool Library::suaSach(const std::string& ma, const std::string& ten_moi, const std::string& tg_moi,
                       const std::string& tl_moi, int sl_moi) {
    Book* b = searchBST(root, ma);
    if (b == nullptr) {
        std::cout << "[Loi] Khong tim thay sach ma " << ma << "\n";
        return false;
    }
    HistoryAction* act = new HistoryAction(SUA_SACH, ma, "", getCurrentTime());
    act->old_ten = b->ten_sach;
    act->old_tac_gia = b->tac_gia;
    act->old_the_loai = b->the_loai;
    act->old_so_luong = b->so_luong;
    history.push(act);

    int da_muon = b->so_luong - b->so_luong_con;
    b->ten_sach = ten_moi;
    b->tac_gia = tg_moi;
    b->the_loai = tl_moi;
    b->so_luong = sl_moi;
    b->so_luong_con = sl_moi - da_muon; // giữ nguyên số đang được mượn
    if (b->so_luong_con < 0) b->so_luong_con = 0;

    std::cout << "[OK] Da cap nhat sach ma " << ma << "\n";
    return true;
}

// ---------- 2. Tìm kiếm ----------
Book* Library::timTheoMa(const std::string& ma) const {
    return searchBST(root, ma);
}

std::vector<Book*> Library::timTheoTen(const std::string& ten) const {
    std::vector<Book*> result;
    searchByNameHelper(root, ten, result);
    return result;
}

// ---------- Quản lý độc giả ----------
bool Library::themDocGia(const std::string& ma, const std::string& ten, const std::string& sdt) {
    if (findReader(ma) != nullptr) {
        std::cout << "[Loi] Ma doc gia '" << ma << "' da ton tai!\n";
        return false;
    }
    Reader* r = new Reader(ma, ten, sdt);
    r->next = readerHead;
    readerHead = r;
    std::cout << "[OK] Da them doc gia: " << ten << " (" << ma << ")\n";
    return true;
}

void Library::inDanhSachDocGia() const {
    if (readerHead == nullptr) {
        std::cout << "(Chua co doc gia nao)\n";
        return;
    }
    Reader* cur = readerHead;
    while (cur != nullptr) {
        std::cout << "- " << cur->ma_doc_gia << " | " << cur->ho_ten
                  << " | SDT: " << cur->sdt
                  << " | Dang muon: " << cur->so_sach_dang_muon
                  << " | Tong luot muon: " << cur->tong_luot_muon << "\n";
        cur = cur->next;
    }
}

// ---------- 3. Mượn sách ----------
bool Library::muonSach(const std::string& ma_sach, const std::string& ma_doc_gia) {
    Book* b = searchBST(root, ma_sach);
    if (b == nullptr) {
        std::cout << "[Loi] Khong tim thay sach ma " << ma_sach << "\n";
        return false;
    }
    Reader* r = findReader(ma_doc_gia);
    if (r == nullptr) {
        std::cout << "[Loi] Khong tim thay doc gia ma " << ma_doc_gia << "\n";
        return false;
    }

    if (b->so_luong_con > 0) {
        b->so_luong_con--;
        b->luot_muon++;
        r->so_sach_dang_muon++;
        r->tong_luot_muon++;

        HistoryAction* act = new HistoryAction(MUON_SACH, ma_sach, ma_doc_gia, getCurrentTime());
        history.push(act);

        std::cout << "[OK] " << r->ho_ten << " da muon sach '" << b->ten_sach << "'\n";
    } else {
        b->hang_cho.push(ma_doc_gia);
        std::cout << "[Thong bao] Sach '" << b->ten_sach << "' da het. "
                  << r->ho_ten << " duoc dua vao hang cho (vi tri "
                  << b->hang_cho.size() << ")\n";
    }
    return true;
}

// ---------- 4. Trả sách ----------
bool Library::traSach(const std::string& ma_sach, const std::string& ma_doc_gia) {
    Book* b = searchBST(root, ma_sach);
    if (b == nullptr) {
        std::cout << "[Loi] Khong tim thay sach ma " << ma_sach << "\n";
        return false;
    }
    Reader* r = findReader(ma_doc_gia);
    if (r == nullptr) {
        std::cout << "[Loi] Khong tim thay doc gia ma " << ma_doc_gia << "\n";
        return false;
    }
    if (r->so_sach_dang_muon <= 0) {
        std::cout << "[Loi] Doc gia nay khong dang muon sach nao.\n";
        return false;
    }

    r->so_sach_dang_muon--;

    HistoryAction* act = new HistoryAction(TRA_SACH, ma_sach, ma_doc_gia, getCurrentTime());
    history.push(act);

    std::cout << "[OK] " << r->ho_ten << " da tra sach '" << b->ten_sach << "'\n";

    if (!b->hang_cho.isEmpty()) {
        std::string ma_nguoi_cho = b->hang_cho.pop();
        Reader* nguoi_cho = findReader(ma_nguoi_cho);
        b->luot_muon++;
        if (nguoi_cho != nullptr) {
            nguoi_cho->so_sach_dang_muon++;
            nguoi_cho->tong_luot_muon++;
            std::cout << "[Thong bao] Sach da duoc tu dong giao cho nguoi cho tiep theo: "
                      << nguoi_cho->ho_ten << " (" << ma_nguoi_cho << ")\n";
        }
        // so_luong_con giữ nguyên (1 cuốn vừa trả lập tức đi cho người chờ)
    } else {
        b->so_luong_con++;
    }
    return true;
}

// ---------- 5. Lịch sử & Undo ----------
void Library::xemLichSu() const {
    auto list = history.toVector();
    if (list.empty()) {
        std::cout << "(Chua co thao tac nao)\n";
        return;
    }
    std::cout << "--- Lich su thao tac (moi nhat truoc) ---\n";
    for (auto act : list) {
        std::string ten_loai;
        switch (act->type) {
            case THEM_SACH: ten_loai = "THEM SACH"; break;
            case XOA_SACH: ten_loai = "XOA SACH"; break;
            case SUA_SACH: ten_loai = "SUA SACH"; break;
            case MUON_SACH: ten_loai = "MUON SACH"; break;
            case TRA_SACH: ten_loai = "TRA SACH"; break;
        }
        std::cout << "[" << act->thoi_gian << "] " << ten_loai
                  << " | Ma sach: " << act->ma_sach;
        if (!act->ma_doc_gia.empty()) std::cout << " | Doc gia: " << act->ma_doc_gia;
        std::cout << "\n";
    }
}

bool Library::undoLast() {
    if (history.isEmpty()) {
        std::cout << "[Thong bao] Khong co thao tac nao de Undo.\n";
        return false;
    }
    HistoryAction* act = history.pop();

    switch (act->type) {
        case THEM_SACH: {
            // undo thêm sách -> xóa sách đó (không ghi lại lịch sử mới)
            root = deleteBST(root, act->ma_sach);
            std::cout << "[Undo] Da huy thao tac THEM SACH (" << act->ma_sach << ")\n";
            break;
        }
        case XOA_SACH: {
            // undo xóa sách -> thêm lại với dữ liệu cũ
            Book* b = new Book(act->ma_sach, act->old_ten, act->old_tac_gia,
                                act->old_the_loai, act->old_so_luong);
            root = insertBST(root, b);
            std::cout << "[Undo] Da khoi phuc sach bi xoa (" << act->ma_sach << ")\n";
            break;
        }
        case SUA_SACH: {
            Book* b = searchBST(root, act->ma_sach);
            if (b != nullptr) {
                int da_muon = b->so_luong - b->so_luong_con;
                b->ten_sach = act->old_ten;
                b->tac_gia = act->old_tac_gia;
                b->the_loai = act->old_the_loai;
                b->so_luong = act->old_so_luong;
                b->so_luong_con = act->old_so_luong - da_muon;
                if (b->so_luong_con < 0) b->so_luong_con = 0;
            }
            std::cout << "[Undo] Da khoi phuc thong tin sach truoc khi sua (" << act->ma_sach << ")\n";
            break;
        }
        case MUON_SACH: {
            Book* b = searchBST(root, act->ma_sach);
            Reader* r = findReader(act->ma_doc_gia);
            if (b != nullptr) { b->so_luong_con++; b->luot_muon--; }
            if (r != nullptr) { r->so_sach_dang_muon--; r->tong_luot_muon--; }
            std::cout << "[Undo] Da huy thao tac MUON SACH (" << act->ma_sach << ")\n";
            break;
        }
        case TRA_SACH: {
            Book* b = searchBST(root, act->ma_sach);
            Reader* r = findReader(act->ma_doc_gia);
            if (b != nullptr && b->so_luong_con > 0) b->so_luong_con--;
            if (r != nullptr) r->so_sach_dang_muon++;
            std::cout << "[Undo] Da huy thao tac TRA SACH (" << act->ma_sach << ")\n";
            break;
        }
    }
    delete act;
    return true;
}

// ---------- 6. Thống kê ----------
void Library::thongKeSachMuonNhieuNhat() const {
    std::vector<Book*> all;
    inorderCollect(root, all);
    if (all.empty()) {
        std::cout << "(Chua co sach nao)\n";
        return;
    }
    Book* best = all[0];
    for (auto b : all) if (b->luot_muon > best->luot_muon) best = b;
    std::cout << "Sach duoc muon nhieu nhat: " << best->ten_sach
              << " (" << best->ma_sach << ") - " << best->luot_muon << " luot muon\n";
}

void Library::thongKeDocGiaTichCucNhat() const {
    if (readerHead == nullptr) {
        std::cout << "(Chua co doc gia nao)\n";
        return;
    }
    Reader* best = readerHead;
    Reader* cur = readerHead;
    while (cur != nullptr) {
        if (cur->tong_luot_muon > best->tong_luot_muon) best = cur;
        cur = cur->next;
    }
    std::cout << "Doc gia tich cuc nhat: " << best->ho_ten
              << " (" << best->ma_doc_gia << ") - " << best->tong_luot_muon << " luot muon\n";
}

// ---------- 7. Lưu / đọc file ----------
bool Library::luuFile(const std::string& filename) const {
    std::ofstream fout(filename);
    if (!fout.is_open()) {
        std::cout << "[Loi] Khong the mo file de ghi: " << filename << "\n";
        return false;
    }
    std::vector<Book*> all;
    inorderCollect(root, all);

    fout << "#SACH\n";
    for (auto b : all) {
        // Thay ky tu xuong dong trong tom_tat/noi_dung_online bang khoang trang
        // de khong lam vo dinh dang file (moi ban ghi = 1 dong).
        std::string tt = b->tom_tat, ndol = b->noi_dung_online;
        std::replace(tt.begin(), tt.end(), '\n', ' ');
        std::replace(ndol.begin(), ndol.end(), '\n', ' ');
        fout << b->ma_sach << "|" << b->ten_sach << "|" << b->tac_gia << "|"
             << b->the_loai << "|" << b->so_luong << "|" << b->so_luong_con
             << "|" << b->luot_muon << "|" << tt << "|" << ndol << "\n";
    }

    fout << "#DOCGIA\n";
    Reader* r = readerHead;
    while (r != nullptr) {
        fout << r->ma_doc_gia << "|" << r->ho_ten << "|" << r->sdt << "|"
             << r->so_sach_dang_muon << "|" << r->tong_luot_muon << "\n";
        r = r->next;
    }

    fout << "#BINHLUAN\n";
    for (auto b : all) {
        for (auto c : b->binh_luan.toVector()) {
            std::string nd = c->noi_dung;
            std::replace(nd.begin(), nd.end(), '\n', ' ');
            std::replace(nd.begin(), nd.end(), '~', ' ');
            fout << b->ma_sach << "~" << c->ma_doc_gia << "~" << c->ho_ten_doc_gia << "~"
                 << c->so_sao << "~" << c->thoi_gian << "~" << nd << "\n";
        }
    }

    fout.close();
    std::cout << "[OK] Da luu du lieu vao file " << filename << "\n";
    return true;
}

bool Library::docFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cout << "[Loi] Khong tim thay file: " << filename << "\n";
        return false;
    }
    std::string line;
    int mode = 0; // 1 = sach, 2 = doc gia, 3 = binh luan
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        if (line == "#SACH") { mode = 1; continue; }
        if (line == "#DOCGIA") { mode = 2; continue; }
        if (line == "#BINHLUAN") { mode = 3; continue; }

        if (mode == 3) {
            // binh luan dung dau phan cach '~' thay vi '|'
            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> parts;
            while (std::getline(ss, token, '~')) parts.push_back(token);
            if (parts.size() >= 6) {
                Book* b = searchBST(root, parts[0]);
                if (b != nullptr) {
                    b->binh_luan.push(parts[1], parts[2], parts[5], std::stoi(parts[3]), parts[4]);
                }
            }
            continue;
        }

        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> parts;
        while (std::getline(ss, token, '|')) parts.push_back(token);

        if (mode == 1 && parts.size() >= 7) {
            std::string tt = (parts.size() >= 8) ? parts[7] : "";
            std::string ndol = (parts.size() >= 9) ? parts[8] : "";
            Book* b = new Book(parts[0], parts[1], parts[2], parts[3], std::stoi(parts[4]), tt, ndol);
            b->so_luong_con = std::stoi(parts[5]);
            b->luot_muon = std::stoi(parts[6]);
            root = insertBST(root, b);
        } else if (mode == 2 && parts.size() >= 5) {
            Reader* r = new Reader(parts[0], parts[1], parts[2]);
            r->so_sach_dang_muon = std::stoi(parts[3]);
            r->tong_luot_muon = std::stoi(parts[4]);
            r->next = readerHead;
            readerHead = r;
        }
    }
    fin.close();
    std::cout << "[OK] Da doc du lieu tu file " << filename << "\n";
    return true;
}

// ---------- 8. Thư viện số: tóm tắt, đọc online, bình luận / đánh giá ----------
bool Library::capNhatTomTatVaNoiDung(const std::string& ma, const std::string& tom_tat,
                                      const std::string& noi_dung_online) {
    Book* b = searchBST(root, ma);
    if (b == nullptr) {
        std::cout << "[Loi] Khong tim thay sach ma " << ma << "\n";
        return false;
    }
    if (!tom_tat.empty()) b->tom_tat = tom_tat;
    if (!noi_dung_online.empty()) b->noi_dung_online = noi_dung_online;
    std::cout << "[OK] Da cap nhat tom tat / noi dung online cho sach " << ma << "\n";
    return true;
}

void Library::xemTomTat(const std::string& ma) const {
    Book* b = searchBST(root, ma);
    if (b == nullptr) {
        std::cout << "[Loi] Khong tim thay sach ma " << ma << "\n";
        return;
    }
    std::cout << "--- Tom tat sach: " << b->ten_sach << " (" << b->ma_sach << ") ---\n";
    if (b->tom_tat.empty())
        std::cout << "(Sach nay chua co tom tat)\n";
    else
        std::cout << b->tom_tat << "\n";
}

void Library::docNoiDungOnline(const std::string& ma, bool tuMoTrinhDuyet) const {
    Book* b = searchBST(root, ma);
    if (b == nullptr) {
        std::cout << "[Loi] Khong tim thay sach ma " << ma << "\n";
        return;
    }
    if (b->noi_dung_online.empty()) {
        std::cout << "[Thong bao] Sach '" << b->ten_sach << "' chua co noi dung online.\n";
        return;
    }
    bool laLink = (b->noi_dung_online.rfind("http://", 0) == 0 ||
                   b->noi_dung_online.rfind("https://", 0) == 0);
    if (laLink) {
        std::cout << "--- Doc online: " << b->ten_sach << " ---\n";
        std::cout << "Lien ket: " << b->noi_dung_online << "\n";
        if (tuMoTrinhDuyet) {
#if defined(_WIN32)
            std::string cmd = "start \"\" \"" + b->noi_dung_online + "\"";
#elif defined(__APPLE__)
            std::string cmd = "open \"" + b->noi_dung_online + "\"";
#else
            std::string cmd = "xdg-open \"" + b->noi_dung_online + "\"";
#endif
            std::cout << "(Dang thu mo trinh duyet mac dinh...)\n";
            system(cmd.c_str()); // luu y: chi hoat dong tren may co giao dien do hoa
        } else {
            std::cout << "(Sao chep lien ket tren va mo bang trinh duyet de doc sach)\n";
        }
    } else {
        std::cout << "--- Noi dung sach: " << b->ten_sach << " ---\n";
        std::cout << b->noi_dung_online << "\n";
    }
}

bool Library::themBinhLuan(const std::string& ma_sach, const std::string& ma_doc_gia,
                            const std::string& noi_dung, int so_sao) {
    Book* b = searchBST(root, ma_sach);
    if (b == nullptr) {
        std::cout << "[Loi] Khong tim thay sach ma " << ma_sach << "\n";
        return false;
    }
    Reader* r = findReader(ma_doc_gia);
    if (r == nullptr) {
        std::cout << "[Loi] Khong tim thay doc gia ma " << ma_doc_gia << "\n";
        return false;
    }
    b->binh_luan.push(ma_doc_gia, r->ho_ten, noi_dung, so_sao, getCurrentTime());
    std::cout << "[OK] " << r->ho_ten << " da binh luan ve sach '" << b->ten_sach << "'\n";
    return true;
}

void Library::xemBinhLuan(const std::string& ma_sach) const {
    Book* b = searchBST(root, ma_sach);
    if (b == nullptr) {
        std::cout << "[Loi] Khong tim thay sach ma " << ma_sach << "\n";
        return;
    }
    std::cout << "--- Binh luan / danh gia sach: " << b->ten_sach << " (" << b->ma_sach << ") ---\n";
    if (b->binh_luan.isEmpty()) {
        std::cout << "(Chua co binh luan nao)\n";
        return;
    }
    std::cout << "Diem danh gia trung binh: " << b->binh_luan.diemTrungBinh()
              << "/5 (" << b->binh_luan.size() << " luot danh gia)\n";
    for (auto c : b->binh_luan.toVector()) {
        std::cout << "[" << c->thoi_gian << "] " << c->ho_ten_doc_gia
                  << " (" << c->ma_doc_gia << ") - " << c->so_sao << " sao: "
                  << c->noi_dung << "\n";
    }
}

void Library::xemChiTietSach(const std::string& ma) const {
    Book* b = searchBST(root, ma);
    if (b == nullptr) {
        std::cout << "[Loi] Khong tim thay sach ma " << ma << "\n";
        return;
    }
    std::cout << "======= CHI TIET SACH =======\n";
    std::cout << "Ma sach : " << b->ma_sach << "\n";
    std::cout << "Ten sach: " << b->ten_sach << "\n";
    std::cout << "Tac gia : " << b->tac_gia << "\n";
    std::cout << "The loai: " << b->the_loai << "\n";
    std::cout << "Con lai : " << b->so_luong_con << "/" << b->so_luong << "\n";
    std::cout << "Tom tat : " << (b->tom_tat.empty() ? "(chua co)" : b->tom_tat) << "\n";
    std::cout << "Doc online: " << (b->noi_dung_online.empty() ? "(chua co)" : b->noi_dung_online) << "\n";
    if (b->binh_luan.isEmpty())
        std::cout << "Danh gia: (chua co danh gia nao)\n";
    else
        std::cout << "Danh gia: " << b->binh_luan.diemTrungBinh() << "/5 sao ("
                   << b->binh_luan.size() << " luot)\n";
    std::cout << "==============================\n";
}

void Library::inToanBoSach() const {
    std::vector<Book*> all;
    inorderCollect(root, all);
    if (all.empty()) {
        std::cout << "(Chua co sach nao)\n";
        return;
    }
    std::cout << "--- Danh sach sach (sap xep theo ma - BST inorder) ---\n";
    for (auto b : all) {
        std::cout << b->ma_sach << " | " << b->ten_sach << " | " << b->tac_gia
                  << " | " << b->the_loai << " | Con: " << b->so_luong_con
                  << "/" << b->so_luong << " | Luot muon: " << b->luot_muon;
        if (!b->hang_cho.isEmpty()) std::cout << " | Hang cho: " << b->hang_cho.size();
        std::cout << "\n";
    }
}
