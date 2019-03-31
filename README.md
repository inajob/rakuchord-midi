# RakuChord MIDI

## これはなに？

[RakuChord](https://github.com/inajob/rakuchord)でMIDIを扱うためのファームウェアです

## 制限

- RakuChord自体から音は鳴りません

## 書き込み時の注意

RakuChordにはUSB端子がついており、ここからファームウェアを書き込むことができます。
書き込みを失敗すると最悪の場合RakuChordが動かなくなる可能性があります。自己責任で作業してください。

また、RakuChordのUSB端子は、市販品と比べてもろく、抜き差しを乱暴に行うとコネくタが基板からはがれてしまう場合があります。
いわゆる`もげマイクロ`という現象です。

[![thumbnail](https://pbs.twimg.com/media/Dz7sucGVYAIFRX9.jpg)](https://twitter.com/ina_ani/status/1098575647641948160)

こうなると復活させることが非常に難しいです。USBケーブルの抜き差しはまっすぐに行ってください。

## このファームウェアの書き込み方

RakuChordをPCに接続しこのリポジトリのファームウェアを書き込みます。
（これにより以前の機能は失われます。再びオリジナルのファームウェアを書き込むことで、元の挙動に戻ります。）

Platformioをインストールした状態で

```
# GitHubからrakuchord-midiをclone
$ git clone git@github.com:inajob/rakuchord-midi.git

# firmwareディレクトリへ移動
$ cd firmware

# ファームウェアのビルド＋書き込み
$ platformio run
```

を実行することで、コンパイルとファームウェアの書き込みが行われます。

## MIDI入力として利用する方法（Windows10の場合）

### The Hairless MIDI to Serial Bridge

1. http://projectgus.github.io/hairless-midiserial/ をインストールします
1. RakuChordをPCに接続するとSerial portの項目に「USB-SERIAL CH340 ...」が現れるので選択します。
1. MIDI Outに”Microsoft GS Wavetable Synth”を選択します。
1. RakuChordのスイッチを押して、PCから音が鳴るかを確認します。

手元の環境ではこの方式では遅延が大きすぎて演奏には堪えません。これはMicrosoft GS Wavetable Synthのレイテンシが大きいことに由来します。
そこで、軽量なソフトウェアMIDI音源をインストールします。

### Virtual MIDI Synthのインストール

1. https://coolsoft.altervista.org/en/virtualmidisynth をインストールします。
1. SoundFontをダウンロードします。（何でも良いのですが、試したのは[GeneralUser GS 1.471](http://www.schristiancollins.com/generaluser.php)です）
1. Virtual MIDI Synthの設定の「SoundFontタブ」から上記でダウンロードしたSoundFontを登録します（拡張子`sf2`のファイルを登録します）
1. Virtual MIDI Synthの設定の「オプションタブ」の「追加の出力バッファ」の値を小さめ（手元では50msにしました）に設定します
1. The Hairless MIDI to Serial BridgeのMIDI Outの設定を`VirtualMIDISynth #1`に設定します
1. RakuChordのスイッチを押して、PCから音が鳴るかを確認します

手元の環境では、これでそれらしく演奏することができました。

### リアルタイム入力として使う場合

1. [loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html)をインストールします。
1. Hairless MIDI to Serial BridgeのMIDI Outを`loopMIDI Port`に設定します
1. MIDI入力に対応したシーケンサなどのソフトで、MIDI-INとして `loopMIDI Port`を設定することでMIDI入力デバイスとしてRakuChordを利用することができます。

[![thumbnail](https://pbs.twimg.com/media/D2gYNjTUgAA877x.png)](https://twitter.com/ina_ani/status/1110164460625235968)

## 機能

- RakuChordのSettingスイッチを押しながら、音階スイッチを押すことでMIDIのProgramChange（音色変更）を実行できます

## 注意

- このリポジトリの内容は予告なくアップデートされ、機能に変更が加わる可能性があります。
- このリポジトリの内容を試すことでRakuChordに不具合が生じる可能性がありますが、作者は責任を取れませんので、自己責任で実行してください。



