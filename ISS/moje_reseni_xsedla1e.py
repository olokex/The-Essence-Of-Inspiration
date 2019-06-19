#!/usr/bin/env python
# coding: utf-8

# In[46]:


# ISS projekt 2018/19
# xsedla1e, Adam Sedláček

# 1. ukol
from numpy.fft import fft
import numpy as np
import soundfile
data, fs = soundfile.read('xsedla1e.wav')


# In[2]:


samples = len(data)


# In[3]:


print(f"Počet vzorků: {samples}")


# In[4]:


print(f"Vzorkovací frekvence je: {fs} Hz")


# In[5]:


print(f"Počet binárních vzorků je {samples/16}")


# In[6]:


print(f"Délka vzorku zvuku je {samples/fs} s")


# In[7]:


# 2. ukol
import matplotlib.pyplot as plt
import matplotlib


# In[8]:


decoded = [1 if data[x] > 0 else 0 for x in range(7, len(data), 16)]


# In[9]:


x1 = np.linspace(0, 20, num=320)
plt.plot(x1, data[:320])
x2 = np.linspace(0.5, 19.5, num=20)
plt.stem(x2, decoded[:20], linefmt='r1--', markerfmt='ro')
plt.ylabel("hodnota")
plt.xlabel("ms")
plt.savefig('2_uloha_graf.pdf')
plt.show()


# In[10]:


# 3. ukol
from scipy.signal import tf2zpk
import matplotlib.lines as mlines
B = [0.0192, -0.0185, -0.0185, 0.0192];
A = [1.0000, -2.8870, 2.7997, -0.9113];
zeros = np.roots(B)
poles = np.roots(A)

fig = plt.figure(figsize=(5,5))
ax = fig.add_subplot(111)
ax.grid(linestyle="dashed")
ax.set_axisbelow(True)
ax.set_title("Počítání nul a pólů z A, B")
ax.set_xlabel("Re")
ax.set_ylabel("Im")
ax.set_xlim(-1.5, 1.5)
ax.set_ylim(-1.5, 1.5)

unit_circle = plt.Circle((0, 0), 1, fill=False, color="blue", linewidth=2)
ax.add_artist(unit_circle)

green = mlines.Line2D([], [], color='green', marker='o', markersize=10, label='póly')
red = mlines.Line2D([], [], color='red', marker='x', markersize=10, label='nuly')

ax.legend(handles=[green, red])
ax.scatter(poles.real, poles.imag, marker='o', color='green', s=60).set_zorder(2)
ax.scatter(zeros.real, zeros.imag, marker='x', color='red', s=60).set_zorder(2)

plt.savefig('3_uloha_graf.pdf')
plt.show()

if (poles.size == 0 or np.all(np.abs(poles)) <= 1):
    print("Filtr je stabilní")
else:
    print("Filtr je nestabilní")


# In[11]:


# 4. ukol
from scipy.signal import freqz
w, h = freqz(B, A, fs)
w *= fs / (2 * np.pi)
 
fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(w, np.absolute(h))
ax.set_title("Kmitoctova charakteristika filtru")
ax.set_xlabel("Frekvence [Hz]")
ax.set_ylabel("Modul")
ax.grid(True, linestyle="dashed")
ax.scatter(517, 0, s=25, color="red").set_zorder(3)

plt.savefig('4_uloha_graf.pdf')
plt.show()
print("Jedna se o dolní propust.\nNízké frekvence se propouští > 0.1 a vysoké zadržuje < 0.1")


# In[12]:


from scipy.signal import argrelextrema
a = argrelextrema(np.absolute(h), np.less)[0][0]
print(f"Mezní frekvence je {a} Hz")


# In[13]:


# 5. uloha
from scipy.signal import lfilter
B = [0.0192, -0.0185, -0.0185, 0.0192];
A = [1.0000, -2.8870, 2.7997, -0.9113];


# In[14]:


sdata = lfilter(B, A, data)
fig = plt.figure(figsize=(30, 5), dpi=300)
ax = fig.add_subplot(111)
ax.plot(sdata)
ax.plot(data,alpha=0.5)
ax.set_title("signal s[n] a ss[n]")
ax.set_xlabel("ms")
ax.set_ylabel("hodnoty")
plt.savefig('5_uloha_graf.pdf')
plt.show()


# In[15]:


fig = plt.figure(figsize=(20,5))
ax = fig.add_subplot(111)
x1 = np.linspace(0, 30, num=480)
ax.plot(x1, data[:480], color="yellow")
ax.plot(x1, sdata[:480], alpha=0.5, color="blue")
print("Kvůli přehlednosti vyplotění jen 30 ms")
ax.set_facecolor('xkcd:grey')
ax.grid(True)
ax.set_title("Oba signaly pri 30 ms")
ax.set_xlabel("ms")
ax.set_ylabel("hodnoty")
plt.savefig("5_uloha_graf_blizsi.pdf")
plt.show()


# In[47]:


fig = plt.figure(figsize=(20,5))
ax = fig.add_subplot(111)
x1 = np.linspace(0, 30, num=480)
ax.plot(x1, data[:480], color="yellow")
ax.plot(x1, np.roll(sdata[:480], -15), alpha=0.5, color="blue")
print("Kvůli přehlednosti vyplotění jen 30 ms, který jsem posunul o 15 vzorků vlevo (předběh)")
ax.set_facecolor('xkcd:grey')
ax.grid(True)
ax.set_title("Oba signaly pri 30 ms s ponusem")
ax.set_xlabel("ms")
ax.set_ylabel("hodnoty")
plt.savefig("5_uloha_graf_blizsi_posun.pdf")
plt.show()


# In[17]:


# 6. uloha
ss_shifted_data = np.roll(sdata, -15)
shifted_decoded = [1 if ss_shifted_data[x] > 0 else 0 for x in range(7, len(sdata), 16)]

fig = plt.figure(figsize=(20,5))
ax = fig.add_subplot(111)

x1 = np.linspace(0, 20, num=320)
x2 = np.linspace(0.5, 19.5, num=20)

ax.plot(x1, data[:320], color="orange")
ax.plot(x1, sdata[:320], color="blue")
ax.plot(x1, ss_shifted_data[:320], color="purple")
ax.stem(x2, shifted_decoded[:20], linefmt='r2--', markerfmt='ro')

ax.set_ylabel("hodnoty")
ax.set_xlabel("ms")

orange = mlines.Line2D([], [], color='orange', markersize=10, label='s[n]')
red = mlines.Line2D([], [], color='red', marker='o', markersize=10, label='ss shifted binary[n]')
purple = mlines.Line2D([], [], color='purple', markersize=10, label='ss_shifted[n]')
blue = mlines.Line2D([], [], color='blue', markersize=10, label='ss[n]')

ax.legend(handles=[orange, red, purple, blue])

ax.grid(True)
plt.savefig("6_uloha.pdf")
plt.show()


# In[32]:


# 7. uloha

cnt = sum(1 for i in range(len(decoded)) if decoded[i] != shifted_decoded[i])
print(f"Počet chyb: {cnt} na {len(decoded)} vzorků\nChybovost je {cnt/len(decoded)*100} %")


# In[19]:


# 8. uloha
fourier = fft(data) / len(data)
fourier = fourier[:int(len(fourier)/2)]
fourier = np.abs(fourier)

fourier_ss = fft(sdata) / len(sdata)
fourier_ss = fourier_ss[:int(len(fourier_ss)/2)]
fourier_ss = np.abs(fourier_ss)

freq = np.linspace(0, fs/2, len(fourier_ss))

fig = plt.figure(figsize=(30,5))
ax = fig.add_subplot(111)
ax.plot(freq, fourier)
ax.plot(freq, fourier_ss, alpha=0.7)
ax.set_xlabel("Frekvence [Hz]")
ax.set_ylabel("Modul")
ax.set_title("Porovnání spektra s[n] a ss[n]")
fig.savefig("8_uloha.pdf")
fig.show()


# In[45]:


# 9. uloha
hist, x = np.histogram(data, 50)
fig = plt.figure()
ax = fig.add_subplot(111)
plt.plot(x[:-1], hist/sum(hist))
ax.set_title("Hustota rozdělení pravděpodobnosti")
fig.savefig("9_uloha.pdf")
fig.show()


# In[21]:


# oveření integrálu že je 1
print(np.sum(hist * fs**-1/2))
print((hist/sum(hist)).sum())


# In[22]:


# 10. ukol

# snažil jsem se použit funkci xcor v matlib knihovně, bohužel se mi nepovedlo dosáhnout výsledků, které jsem očekával

cor = []
for i in range(-50,51):
    suma = 0
    for j in range(samples-51):
        suma += data[j]*data[j+i]
    cor.append(suma/len(data))
    
fig = plt.figure()
ax = fig.add_subplot(111)
x = np.linspace(-50, 51, num=101)
plt.plot(x, cor)
ax.set_title("Korelační koeficient R[k]; k ∈ <-50, 50>")
fig.savefig("10_uloha.pdf")
plt.show()


# In[23]:


# 11. uloha
print(f"R[0]: {cor[50]}\nR[1]: {cor[51]}\nR[16]: {cor[66]}")


# In[24]:


# 12. uloha
bin_count = 50
bins, xedges, yedges = np.histogram2d(data[:-1], data[1:]*-1, bin_count, normed=True)
bin_size = xedges[1] - xedges[0]
bin_surface = bin_size**2
 
fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_title("Časový odhad sdružené funkce hustoty \nrozdělení pravděpodobnosti pro n a n+1")
cax = ax.imshow(bins, interpolation='nearest', extent=[xedges[-1], xedges[0], yedges[-1], yedges[0]])
cbar = fig.colorbar(cax)
fig.savefig("12_uloha.pdf")
fig.show()


# In[25]:


# 13. uloha
binsize = np.abs(xedges[0] - xedges[1]) * np.abs(yedges[0] - yedges[1])
integral = np.sum(bins * binsize)
print(integral)


# In[26]:


# 14. ukol
x1_e=xedges[:50]
x2_e=yedges[:50]
print("Korelační koeficient pro R[1] je {0:.4}".format(sum(sum(x1_e * x2_e * bins)*binsize)))


# In[ ]:




