k = int(input('type the number of rotation:')) #3
number = int(input("the number of elements:")) #7
num = [] # 1, 2, 3, 4, 5, 6, 7
cortege = [] # 1, 2, 3, 4, 5, 6, 7
#the first way
"""
for i in range(number):
    current_element = int(input())
    num.append(current_element)
    cortege.append(current_element)

for i in range(number):
    var = i + k - number
    if i+k >= number:
        num[var] = cortege[var+k+1]
    else:
        num[i+k] = cortege[i]

print(num)
"""
#the second way
"""
num[-k:] = num[len(num) - k:]
num[:-k] = num[:len(num) - k]

new_num = num[-k:] + num[:-k]
print(new_num)
"""
