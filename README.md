<div align="center">

# Thuật toán lập thời khóa biểu

</div>

---
- [Giới thiệu](#giới-thiệu)
- [Cấu trúc tập tin](#cấu-trúc-tập-tin)
- [Tiêu chí đánh giá chất lượng](#tiêu-chí-đánh-giá-chất-lượng)
- [Giải thuật](#giải-thuật)
- [Thực nghiệm](#thực-nghiệm)
---

## Giới thiệu
Thuật toán giúp tự động sắp xếp thời khóa biểu THPT cho học sinh và giáo viên. Chất lượng của thời khóa biểu được đánh giá dựa trên nhiều ràng buộc cứng và mềm dựa trên dữ liệu thực tế. *Đây là thuật toán được cài đặt hỗ trợ cho bài báo khoa học của học viên cao học Nguyễn Hồ Thiên Đăng và TS. Phan Tấn Quốc.*

## Cấu trúc tập tin

**Chương trình chính**: ```main.cpp```

**Input Files**:

(Dữ liệu mẫu - *Trường THPT Ngô Quyền, Q.7, TP.HCM*)

```PC_HK1.txt```
```PC_HK2.txt```
(Danh sách phân công của học kì I và II).

```
cấu trúc file: <Mã PC> <Lớp> <Môn> <Mã GV> <Số Tiết> <Cụm>
vd: PC001 10A Anh GV1024 3 2
```

**Output Files**:
- TKB của lớp: ```TKB_lop.txt```
- TKB của gv: ```TKB_gv.txt```
- Vi phạm của lớp: ```VP_lop.txt```
- Vi phạm của gv: ```VP_gv.txt```
- Vi phạm chi tiết: ```VP_chitiet.txt```
- Vi phạm tổng: ```VP_tong.txt```
- Số liệu về điểm vi pham (Cải Thiện 2): ```stat.txt```

## Tiêu chí đánh giá chất lượng
Chất lượng của một thời khóa biểu thể hiện qua thời khóa biểu của các lớp học và thời khóa biểu của mỗi giáo viên. Việc đánh giá chất lượng một thời khóa biểu là công việc khó khăn và thông thường giáo viên sẽ đánh giá dựa vào thông tin thời khóa biểu có vi phạm các ràng buộc của bài toán

### Danh sách các điều kiện ràng buộc
Chi tiết: 

*Trọng số các vi phạm ràng buộc cứng và ràng buộc mềm*
| STT | Tên ràng buộc                                                        | Trọng số |
|-----|----------------------------------------------------------------------|----------|
| 1   | H1: Ràng buộc đụng độ giáo viên                                      | 999      |
| 2   | H2: Ràng buộc đụng độ lớp học                                        | 999      |
| 3   | H3: Ràng buộc đụng độ phòng học                                      | 999      |
| 4   | H4: Ràng buộc về môn học chỉ học 1 lần trong một buổi                | 600      |
| 5   | H5: Ràng buộc tiết lủng của lớp theo buổi                            | 600      |
| 6   | H6: Ràng buộc về tiết không xếp của giáo viên                        | 600      |
| 7   | H7: Ràng buộc về môn học có giới hạn số giáo viên cùng dạy đồng thời | 600      |
| 8   | S1: Ràng buộc về lịch bận của các giáo viên                          | 20       |
| 9   | S2: Ràng buộc về học cách ngày các môn học trong một lớp             | 20       |
| 10  | S3: Ràng buộc về độ nén lịch dạy của giáo viên                       | 10       |
| 11  | S4: Ràng buộc về tiết lủng của giáo viên theo buổi học               | 5        |
| 12  | S5: Ràng buộc về số buổi chỉ dạy 1 tiết trong tuần của giáo viên     | 10       |
| 13  | S6: Ràng buộc về số tiết tối thiểu trong một vuổi học của một lớp    | 20       |
| 14  | S7: Ràng buộc về số môn học tối đa trong mỗi buổi học                | 10       |

### Công thức điểm vi phạm (càng nhỏ càng tốt)

Điểm vi phạm dựa vào hệ số vi phạm và số lần vi phạm của từng ràng buộc.

### Giải thuật


