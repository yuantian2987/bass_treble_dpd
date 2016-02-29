% ����������t_basssin/t_treblesin/t_basscos/t_treblesin�ĸ�������㷨 %
clear all;
%f=250;
%%%250Hz��ʾbass��ֵֹ��2000Hz/4000Hz��ʾtreble��ֵֹf=250ʱ����t_basssin��t_basscos f=2000ʱ����t_treblesin��t_treblecos%
f=2000;%4000Hz
fs= [32000 48000 70100 96000 192000 44100 88200 176400];
w=(2*pi*f)./fs; 
a = sin(w);
b = cos(w);
fixA = (round(a*power(2,16)));
fixB = round(b*power(2,16));

plot(fs, fixA, '-r')
%xlswrite('sin_bass.xlsx',fixA);% t_basssin value in process_bass_treble_table.h
xlswrite('sin_treble.xlsx',fixA);% t_treblesin value in process_bass_treble_table.h
hold on

plot(fs, fixB, '-b')
%xlswrite('cos_bass.xlsx',fixB);% t_basscos value in process_bass_treble_table.h
xlswrite('cos_treble.xlsx',fixB);% t_treblecos value in process_bass_treble_table.h
hold on
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%end

