# dicom_header_reader

file은 binary
vector에는 1byte씩(8bit)
1byte 두 개 묶어서 16bit로

explicit vr 명시적
group|element|vr|length|value

implicit vr 암시적
group|element|length|value

byte order: group, element, length, value는 케바케

Transfer Syntax UID로 explicit, implicit 결정

이 비트 연산자들은 1byte씩 나누기 위해서 진행함

```cpp
if (buffer[i] == (group >> 8) && buffer[i + 1] == (group & 0xFF) &&
    buffer[i + 2] == (element >> 8) && buffer[i + 3] == (element & 0xFF))
```

```
bit 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0
    |-----상위 바이트-----|  |-----하위 바이트-----|

  0001 0010 0011 0100  (0x1234)
& 0000 0000 1111 1111  (0xFF)
  -------------------
  0000 0000 0011 0100  (0x34)
```

```
>>>>>>>>big endian
00 00 00 01
<<<<<<<<little endian
```

근데 파일이 big인지 little인지 확인하려면 transfer syntax uid를 봐야하는데
그럼 trasfer syntax(0x0002, 0x0010)는 어딨는지 어떻게 알수 있음??
-> metadata는 always little endian
metadata가 explicit인지 implicit인지도 transfer syntax uid 봐야하는데??
-> 동일함

2바이트 Length 필드를 사용하는 VR:
AE, AS, AT, CS, DA, DS, DT, FL, FD, IS, LO, LT, PN, SH, SL, SS, ST, TM, UI, UL, US

4바이트 Length 필드를 사용하는 VR:
OB, OW, OF, SQ, UT, UN

비트연산 살짝 벽느낌

vr마다 length도 달라지고, value의 data type도 바뀜

metadata는 전부 가져와서 저장시켜놓는게
TagInfo에 넣고, 그걸 tag_map에 넣어가야함.
