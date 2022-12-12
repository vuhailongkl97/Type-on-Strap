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
+ Đảm bảo phía client sẽ không phải handle exceptions vì internal function được gọi đã handle ví dụ hàm đó chỉ trả về true or false thay vì exceptions.
### Item 13 ví dụ về exception guarantee
+ copy and swap idiom. nhằm tránh việc duplicate code với copy constructor, không cần quan tâm việc delete với point cũ trong this pointer( đã automatically calling the destructor cho tmp obj) và đồng thời đảm bảo exception safety vì dòng 2,3 nào can't throw. Chỉ duy nhất việc copy vào *other* nếu failed thì sẽ xảy ra exception và phía client cần handle điều đó.
```
    Derived & operator = (Derived other) {
        swap(this->p , other.p);
        return *this;
    }
```
khi so sánh với các làm thông thường bên dưới.
```
  Derived & operator = (const Derived &other) {
    if( this != &other )
    {
      T* v_new = NewCopy( other.v_,
      other.vsize_,
      other.vsize_ );
      delete[] v_; // this can't throw
      v_ = v_new; // take ownership
      vsize_ = other.vsize_;
      vused_ = other.vused_;
    }
    return *this; // safe, no copy involved
  }
```

### Item 15: try/catch
+ Việc throw lại 1 exception mới có thể dẫn đến side effect vì client source code có thể đã handle exception cũ do vậy cần chú ý, hạn chế throw khi không cần thiết.
+ Việc define các exception specification cho 1 class là không cần thiết đặc biệt đối với trường hợp class đó là generic vì ta không thể biết T ở đó là gì và những exception có thể có với khi thao tác với T
```
template <typename T>
class Vector {
 ...
}
```

#### Summary
```
The advice "be aware, drive with care" certainly applies to writing exception-safe code for containers and
other objects. To do it successfully, you do have to meet a sometimes significant extra duty of care. But
don't get unduly frightened by exceptions. Apply the guidelines outlined above—that is, isolate your resource
management, use the "update a temporary and swap" idiom, and never write classes whose destructors can
allow exceptions to escape—and you'll be well on your way to safe and happy production code that is both
exception-safe and exception-neutral. The advantages can be both concrete and well worth the trouble for
your library and your library's users
```

### 