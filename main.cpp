#include "library.h"
#include <iostream>
#include <limits>

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Bộ test case tự động: demo nhanh toàn bộ 7 chức năng
void chayTestCase(Library& lib) {
    std::cout << "\n========== CHAY TEST CASE TU DONG ==========\n";

    std::cout << "\n-- 1. Them sach --\n";
    lib.themSach("S001", "Lap Trinh C++", "Nguyen Van A", "Cong nghe", 2,
                 "Cuon sach nhap mon lap trinh C++ tu co ban den nang cao.",
                 "https://example.com/doc/lap-trinh-cpp");
    lib.themSach("S002", "Cau Truc Du Lieu", "Tran Thi B", "Cong nghe", 1,
                 "Trinh bay cac cau truc du lieu co ban: mang, danh sach lien ket, cay, do thi.",
                 "https://example.com/doc/cau-truc-du-lieu");
    lib.themSach("S003", "Toan Roi Rac", "Le Van C", "Toan hoc", 1);
    lib.themSach("S001", "Trung ma", "X", "Y", 1); // test trung ma -> phai bao loi

    std::cout << "\n-- Them doc gia --\n";
    lib.themDocGia("DG01", "Pham Van D", "0901111111");
    lib.themDocGia("DG02", "Hoang Thi E", "0902222222");
    lib.themDocGia("DG03", "Vu Van F", "0903333333");

    std::cout << "\n-- 2. Tim kiem --\n";
    Book* b = lib.timTheoMa("S002");
    if (b) std::cout << "Tim theo ma S002: " << b->ten_sach << "\n";
    auto ketqua = lib.timTheoTen("C++");
    std::cout << "Tim theo ten chua 'C++': " << ketqua.size() << " ket qua\n";

    std::cout << "\n-- 3. Muon sach (S002 chi co 1 ban) --\n";
    lib.muonSach("S002", "DG01"); // DG01 muon thanh cong
    lib.muonSach("S002", "DG02"); // S002 het -> DG02 vao hang cho
    lib.muonSach("S001", "DG03");

    std::cout << "\n-- 4. Tra sach (S002) --\n";
    lib.traSach("S002", "DG01"); // se tu dong giao cho DG02 dang cho

    std::cout << "\n-- In toan bo sach sau khi muon/tra --\n";
    lib.inToanBoSach();

    std::cout << "\n-- 5. Xem lich su --\n";
    lib.xemLichSu();

    std::cout << "\n-- 5. Undo thao tac cuoi (Tra sach S002) --\n";
    lib.undoLast();
    lib.inToanBoSach();

    std::cout << "\n-- 6. Thong ke --\n";
    lib.thongKeSachMuonNhieuNhat();
    lib.thongKeDocGiaTichCucNhat();

    std::cout << "\n-- 8. Thu vien so: tom tat / doc online / binh luan --\n";
    lib.xemTomTat("S001");
    lib.docNoiDungOnline("S001"); // chi in link, khong tu mo trinh duyet trong test tu dong
    lib.themBinhLuan("S001", "DG03", "Sach viet de hieu, vi du ro rang!", 5);
    lib.themBinhLuan("S001", "DG02", "Con thieu mot so vi du nang cao.", 4);
    lib.xemBinhLuan("S001");
    lib.xemChiTietSach("S001");

    std::cout << "\n-- 7. Luu file --\n";
    lib.luuFile("thuvien_test.txt");

    std::cout << "\n========== KET THUC TEST CASE ==========\n\n";
}

void hienThiMenu() {
    std::cout << "\n================ HE THONG QUAN LY THU VIEN ================\n";
    std::cout << " 1. Them sach\n";
    std::cout << " 2. Xoa sach\n";
    std::cout << " 3. Sua thong tin sach\n";
    std::cout << " 4. Tim kiem sach (theo ma hoac ten)\n";
    std::cout << " 5. Them doc gia\n";
    std::cout << " 6. Xem danh sach doc gia\n";
    std::cout << " 7. Muon sach\n";
    std::cout << " 8. Tra sach\n";
    std::cout << " 9. Xem lich su thao tac\n";
    std::cout << "10. Undo thao tac cuoi\n";
    std::cout << "11. Thong ke sach muon nhieu nhat\n";
    std::cout << "12. Thong ke doc gia tich cuc nhat\n";
    std::cout << "13. In toan bo danh sach sach\n";
    std::cout << "14. Luu du lieu ra file\n";
    std::cout << "15. Doc du lieu tu file\n";
    std::cout << "16. Chay bo test case tu dong (demo nhanh)\n";
    std::cout << "---------------- THU VIEN SO ----------------\n";
    std::cout << "17. Xem chi tiet sach (tom tat + link + diem danh gia)\n";
    std::cout << "18. Xem tom tat sach\n";
    std::cout << "19. Doc noi dung online\n";
    std::cout << "20. Cap nhat tom tat / link doc online cho sach\n";
    std::cout << "21. Viet binh luan / danh gia sach\n";
    std::cout << "22. Xem binh luan / danh gia sach\n";
    std::cout << " 0. Thoat\n";
    std::cout << "=============================================================\n";
    std::cout << "Chon chuc nang: ";
}

int main() {
    Library lib;
    int luaChon;

    std::cout << "Chao mung den voi He thong Quan ly Thu Vien!\n";
    std::cout << "(Goi y: chon 16 de chay nhanh bo test case demo toan bo chuc nang)\n";

    do {
        hienThiMenu();
        if (!(std::cin >> luaChon)) {
            clearInput();
            std::cout << "[Loi] Vui long nhap so!\n";
            continue;
        }

        switch (luaChon) {
            case 1: {
                std::string ma, ten, tg, tl;
                int sl;
                std::cout << "Ma sach: "; std::cin >> ma;
                clearInput();
                std::cout << "Ten sach: "; std::getline(std::cin, ten);
                std::cout << "Tac gia: "; std::getline(std::cin, tg);
                std::cout << "The loai: "; std::getline(std::cin, tl);
                std::cout << "So luong: "; std::cin >> sl;
                lib.themSach(ma, ten, tg, tl, sl);
                break;
            }
            case 2: {
                std::string ma;
                std::cout << "Ma sach can xoa: "; std::cin >> ma;
                lib.xoaSach(ma);
                break;
            }
            case 3: {
                std::string ma, ten, tg, tl;
                int sl;
                std::cout << "Ma sach can sua: "; std::cin >> ma;
                clearInput();
                std::cout << "Ten sach moi: "; std::getline(std::cin, ten);
                std::cout << "Tac gia moi: "; std::getline(std::cin, tg);
                std::cout << "The loai moi: "; std::getline(std::cin, tl);
                std::cout << "So luong moi: "; std::cin >> sl;
                lib.suaSach(ma, ten, tg, tl, sl);
                break;
            }
            case 4: {
                int kieu;
                std::cout << "Tim theo (1) Ma hay (2) Ten? "; std::cin >> kieu;
                if (kieu == 1) {
                    std::string ma;
                    std::cout << "Nhap ma sach: "; std::cin >> ma;
                    Book* b = lib.timTheoMa(ma);
                    if (b) std::cout << "Tim thay: " << b->ten_sach << " - " << b->tac_gia
                                      << " | Con: " << b->so_luong_con << "/" << b->so_luong << "\n";
                    else std::cout << "Khong tim thay sach.\n";
                } else {
                    std::string ten;
                    clearInput();
                    std::cout << "Nhap ten sach (hoac mot phan ten): "; std::getline(std::cin, ten);
                    auto ds = lib.timTheoTen(ten);
                    if (ds.empty()) std::cout << "Khong tim thay sach.\n";
                    for (auto b : ds)
                        std::cout << "- " << b->ma_sach << ": " << b->ten_sach << "\n";
                }
                break;
            }
            case 5: {
                std::string ma, ten, sdt;
                std::cout << "Ma doc gia: "; std::cin >> ma;
                clearInput();
                std::cout << "Ho ten: "; std::getline(std::cin, ten);
                std::cout << "So dien thoai: "; std::getline(std::cin, sdt);
                lib.themDocGia(ma, ten, sdt);
                break;
            }
            case 6:
                lib.inDanhSachDocGia();
                break;
            case 7: {
                std::string ms, md;
                std::cout << "Ma sach: "; std::cin >> ms;
                std::cout << "Ma doc gia: "; std::cin >> md;
                lib.muonSach(ms, md);
                break;
            }
            case 8: {
                std::string ms, md;
                std::cout << "Ma sach: "; std::cin >> ms;
                std::cout << "Ma doc gia: "; std::cin >> md;
                lib.traSach(ms, md);
                break;
            }
            case 9:
                lib.xemLichSu();
                break;
            case 10:
                lib.undoLast();
                break;
            case 11:
                lib.thongKeSachMuonNhieuNhat();
                break;
            case 12:
                lib.thongKeDocGiaTichCucNhat();
                break;
            case 13:
                lib.inToanBoSach();
                break;
            case 14: {
                std::string filename;
                std::cout << "Ten file (vd: thuvien.txt): "; std::cin >> filename;
                lib.luuFile(filename);
                break;
            }
            case 15: {
                std::string filename;
                std::cout << "Ten file (vd: thuvien.txt): "; std::cin >> filename;
                lib.docFile(filename);
                break;
            }
            case 16:
                chayTestCase(lib);
                break;
            case 17: {
                std::string ma;
                std::cout << "Ma sach: "; std::cin >> ma;
                lib.xemChiTietSach(ma);
                break;
            }
            case 18: {
                std::string ma;
                std::cout << "Ma sach: "; std::cin >> ma;
                lib.xemTomTat(ma);
                break;
            }
            case 19: {
                std::string ma;
                std::cout << "Ma sach: "; std::cin >> ma;
                char tuMo;
                std::cout << "Tu dong mo trinh duyet neu la link? (y/n): "; std::cin >> tuMo;
                lib.docNoiDungOnline(ma, (tuMo == 'y' || tuMo == 'Y'));
                break;
            }
            case 20: {
                std::string ma, tt, ndol;
                std::cout << "Ma sach can cap nhat: "; std::cin >> ma;
                clearInput();
                std::cout << "Tom tat moi (Enter de bo qua): "; std::getline(std::cin, tt);
                std::cout << "Link/Noi dung online moi (Enter de bo qua): "; std::getline(std::cin, ndol);
                lib.capNhatTomTatVaNoiDung(ma, tt, ndol);
                break;
            }
            case 21: {
                std::string ms, md, nd;
                int sao;
                std::cout << "Ma sach: "; std::cin >> ms;
                std::cout << "Ma doc gia: "; std::cin >> md;
                clearInput();
                std::cout << "Noi dung binh luan: "; std::getline(std::cin, nd);
                std::cout << "So sao danh gia (1-5): "; std::cin >> sao;
                lib.themBinhLuan(ms, md, nd, sao);
                break;
            }
            case 22: {
                std::string ma;
                std::cout << "Ma sach: "; std::cin >> ma;
                lib.xemBinhLuan(ma);
                break;
            }
            case 0:
                std::cout << "Tam biet!\n";
                break;
            default:
                std::cout << "[Loi] Lua chon khong hop le!\n";
        }
    } while (luaChon != 0);

    return 0;
}
