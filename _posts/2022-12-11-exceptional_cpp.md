---
published: true
tag:
  - cpp
---
# Tóm tắt Exceptional C Plus Plus
## Sử dụng iterator và thư viện
### Item 1:
sử dụng iterator trong algorithm đúng :
không nên sử dụng phép toán số học lên iterator khi không chắc chắn pointer đó out of range 
ví dụ 
```
std::vector<int> vi;
std::cout << vi.begin() + 1;

for(auto it = vi.begin(); vi != vi.end(); vi++){
	if(*it %2){
    	vi.erase(it);
    }
}
```
ví dụ 1 bug access an element out of the vector's range
ví dụ 2 khi xóa trong lúc duyệt chương trình sẽ không đảm bảo *vi* còn point tới vi nữa hay không ( có thể sau khi xóa vùng nhớ của vi sẽ thay đỏi và ta không chắc chắn được -> vi++ là undefine behavior)

### Item 6: tránh temporary object 
```
string FindAddr( list<Employee> emps, string name ) (1)
{
	for( list<Employee>::iterator i = emps.begin();i != emps.end();i++)(2)
    {
      if( *i == name ) (3)
      {
          return i->addr;
      }
    }
    return "";
}
```
theo đoạn code trên có các điểm làm lãng phí tài nguyên.  
(1). nên dùng const & thay vì copy giá trị  
(2). việc recalculate emps.end() là không cần thiết.  
(3). \*i nghĩa là từ kiểu Employee cần implement việc cast về string -> tạo ra temporary string thay vào đó ta có thể implement 1 method compare cho lớp Employee ví dụ i.compare(name).

Để tránh gặp phải những đoạn code như trên ta nên sử dụng các function đã được define trong std::algorithm như std::find thay vì viết for loop như ví dụ trên, thư viện đã tối ưu và loại bỏ những nhược điểm không đáng có.

## Exception safety
Mô tả các rule, kinh nghiệm viết code an toàn. tránh side effect và những lỗi tiềm ẩn với exception.
một (đoạn) chương trình được đảm bảo exception safety theo các tiêu chí sau:
+ Đảm bảo đoạn chương tình đó sử dụng, giải phóng tài nguyên đúng đắn ngay cả khi exception xảy ra vd (std::bad_alloc, std::runtime,..).
+ Đảm bảo tài nguyên được an toàn ví dụ khi đang copy 1 array 1-> array 2 và exception xảy ra thì cần đảm bảo array2 phải recover được trạng thái trước copy, chỉ được phép copy khi không có bất kì exception nào xảy ra trong quá trình copy  

```
vector<int> arr1, arr2;
copy(arr1.begin(), arr1.end(), arr2.begin()); 
// exception safety by library cho các kiểu dữ kiểu nguyên bản của c++ (int, char, long,...).
Với class khi ta sử dụng copy như trên đoạn code đó sẽ không đảm bảo exception safety.

```
+ 
### Item
