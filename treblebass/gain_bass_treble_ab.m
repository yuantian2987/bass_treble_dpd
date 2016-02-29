% 以下是生成数组gain_a和gain_b的程序，生成的数据直接从 result_a.xlsx/result_b.xlsx中取即可%
clear all;
%gain = -15:0.1:15
gain = -25:0.1:25;%gain的左右范围，间距为0.1
result_aa = 10.^(gain./40.0);
%result_a = (2.^10).*(10.^(gain/40.0));%输出原始数据，保留小数部分
result_a = round((2.^16).*(10.^(gain./40.0)));%输出四舍五入之后的整数，2.^16表示数据左移16位
%plot(gain, result_a, '-r');
xlswrite('result_a.xlsx',result_a');
%hold on

%%result_b = round((2.^16).*sqrt(((result_aa.^2 + 1)./0.4) - (result_aa - 1).^2));
result_b = round((2.^16).*sqrt(((result_aa.^2 + 1)./0.8) - (result_aa - 1).^2));%输出四舍五入之后的整数，2.^16表示数据左移16位
%plot(gain, result_b);
xlswrite('result_b.xlsx',result_b');
%hold on
