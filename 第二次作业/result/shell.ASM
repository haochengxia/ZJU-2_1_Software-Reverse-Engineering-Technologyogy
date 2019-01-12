code segment 
assume cs:code,ds:code
main:
    push ax
    push bx
    push cx
    push dx
    push ds
    push es
    push di
    push si
    push bp
    mov bp,es  ;get the adress for psp
    add bp,10h ;the first address in memory
    call decode
decode:
    pop bx ;BX = actual address of decrypt
    sub bx, offset decode - offset main  ;BX = actual address of main
    push cs ;Push the Code Segment Register
    pop ds ;DS = CS
cal_length:
    lea di,[bx+end_flags]
    add di,1h ;di = actual File header address
    add di,2h 
    mov dx,[di] ;dx = the bytes number of the last section
    mov word ptr [bx+length_decrypt],dx ;save the bytes number of the last section in the length_decrypt
    add di,2h 
    mov ax,[di] ; the exe occuqied sections' number
    mov word ptr [bx+length_decrypt+2],ax
    add di,4h 
    mov ax,[di] ;file header paragraph number
    sub ax,20h ;if file header paragraph number is equal to 20h jump to temp else compute the length of the file head
    jz temp
    mov word ptr [bx+header_length],ax
    mov ax,10h
    mul word ptr [bx+header_length]
    mov dx,word ptr[bx+length_decrypt]
    sub dx,ax
    mov word ptr[bx+length_decrypt],dx
    mov ax,word ptr[bx+length_decrypt+2]
    inc ax
    mov word ptr[bx+length_decrypt+2],ax
temp: 
    mov ax,word ptr [bx+length_decrypt] ;read the last section's length of the code need to be decrypt
    xor ax,0 
    je len_zero  ; if the last section's length of the code is equal to 0, jump
    jmp len_nzero ; if the last section's length of the code is not equal to 0, firstly we loop these times 
len_zero:
    mov cx,word ptr [bx+length_decrypt+2]
    dec cx ;section number -1 
    mov dx,bp 
    mov es,dx ;es = bp (because the code part need to be decrypt is looking as the extra data segemnt, and bp = the first address in memory)
next_zf:
    push cx
    mov cx,200h
    mov di,0
next_zs:
    xor byte ptr es:[di],33h
    inc di
    loop next_zs 
    pop cx
    mov ax,es
    add ax,20h
    mov es,ax
    loop next_zf
    jmp relocation_pre
len_nzero:
    mov dx,bp
    mov es,dx
    mov cx,word ptr [bx+length_decrypt+2]
    dec cx
    dec cx
    xor cx,0
    je ag_pre
next_nf:
    push cx
    mov cx,200h
    mov di,0
next_ns:
    xor byte ptr es:[di],33h
    inc di
    loop next_ns
    pop cx
    mov ax,es
    add ax,20h
    mov es,ax
    loop next_nf
ag_pre:
    mov cx,word ptr [bx+length_decrypt]
    mov di,0
next_ag:
    xor byte ptr es:[di],33h
    inc di
    loop next_ag
;relocation part
relocation_pre:
    lea di,[bx+end_flags]
    add di,1h
    add di,6h
    mov cx,[di]
    xor cx,0
    je gate
    add di,12h
    mov ax,[di]
    lea dx,[bx+end_flags+1]
    add ax,dx
    mov di,ax
    mov ax,[di]
    mov word ptr [bx+relocation_item],ax
reloc:
    mov si,word ptr [bx+relocation_item]
    add di,2h
    mov ax,[di]
    mov word ptr [bx+relocation_item],ax
    add word ptr [bx+relocation_item],bp
    mov ax,word ptr [bx+relocation_item]
    mov es,ax
    add es:[si],bp
    add di,2h
    mov ax,[di]
    mov word ptr [bx+relocation_item],ax
    loop reloc
gate:
    mov ax,word ptr [bx+end_flags+15h]
    mov word ptr [bx+ip_cs],ax
    mov ax,word ptr [bx+end_flags+17h]
    mov word ptr [bx+ip_cs+2],ax
    mov ax,bp
    pop bp
    pop si
    pop di
    pop es
    pop ds
    add word ptr cs:[ip_cs+bx+2],ax
    jmp dword ptr cs:[ip_cs+bx]
    pop dx
    pop cx
    pop bx
    pop ax
ip_cs db 00,00,00,00
header_length db 00,00
relocation_item db 00,00
length_decrypt db 00,00,00,00
end_flags db 00
code ends
end main
