def mk(a):
    ret = ['!']
    for v in a:
        if v!='1' and v!='0':
            return "1 and 0 only"
        cur = (v=='1')
        if cur:
            ret.append('=!')
        ret.append('+')
    ret.append('!')
    for i in range(len(a)):
        ret.append('+')
    return ''.join(ret)




