% 以下是生成t_basssin/t_treblesin/t_basscos/t_treblesin四个数组的算法 %
clear all;
%f=250;
%%%250Hz表示bass截止值，2000Hz/4000Hz表示treble截止值f=250时生成t_basssin和t_basscos f=2000时生成t_treblesin和t_treblecos%
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

