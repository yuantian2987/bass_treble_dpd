% ��������������gain_a��gain_b�ĳ������ɵ�����ֱ�Ӵ� result_a.xlsx/result_b.xlsx��ȡ����%
clear all;
%gain = -15:0.1:15
gain = -25:0.1:25;%gain�����ҷ�Χ�����Ϊ0.1
result_aa = 10.^(gain./40.0);
%result_a = (2.^10).*(10.^(gain/40.0));%���ԭʼ���ݣ�����С������
result_a = round((2.^16).*(10.^(gain./40.0)));%�����������֮���������2.^16��ʾ��������16λ
%plot(gain, result_a, '-r');
xlswrite('result_a.xlsx',result_a');
%hold on

%%result_b = round((2.^16).*sqrt(((result_aa.^2 + 1)./0.4) - (result_aa - 1).^2));
result_b = round((2.^16).*sqrt(((result_aa.^2 + 1)./0.8) - (result_aa - 1).^2));%�����������֮���������2.^16��ʾ��������16λ
%plot(gain, result_b);
xlswrite('result_b.xlsx',result_b');
%hold on
