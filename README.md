# DEM2JSON

## �T�v

Javascript�ňܓx�o�x����W�����擾����X�N���v�g�p�̕W���f�[
�^���쐬����v���O����

## ����

���̃c�[����Javascript�ŗ��p�ł���W���f�[�^���쐬���܂��B�W
���f�[�^��"DEM (Digital Elevation Model)"�ƌĂ΂��W���f�[�^
����쐬���܂��B�t���[��DEM�͂Q�قǑ��݂��܂��B�P��[GTOPO30]
�ƌĂ΂����̂ł����A�i�������܂�悭����܂���B�������
[ASTER]�ƌĂ΂����̂ŕi���͍����ł��B�����̃f�[�^�̓���
���@�ɂ��Ă͂����ł͏q�ׂ܂���̂ŁA�ڍׂ͊e�T�C�g���Q�Ƃ�
�Ă��������B
�쐬���ꂽ�W���f�[�^��JSON�`����Javascript���痘�p���邱�Ƃ�
�ł��܂��B
�\�[�X�ɂ̓R���o�[�^��Javascript�p��js�t�@�C�����܂܂�܂��B

  [GTOPO30]: http://eros.usgs.gov/#/Find_Data/Products_and_Data_Available/gtopo30_info
  [ASTER]: http://asterweb.jpl.nasa.gov/

## �r���h���@

MinGW���gcc�ŃR���p�C���ł��Ă��邱�Ƃ��m�F���Ă��܂��B
���̑��̊��ɂ��Ă͖��m�F�ł��B
�r���h�ɂ�[zlib] [minizip] [boost]���C�u�������K�v�ł��B

  [zlib]: http://zlib.net/
  [minizip]: http://www.winimage.com/zLibDll/minizip.html
  [boost]: http://www.boost.org/

�t�H���_�\��:

    DEM2JSON/
        minizip/
            ioapi.c
            ioapi.h
            unzip.c
            unzip.h
        src/
            AltitudeMatrix.cpp
            AltitudeMatrix.h
            CommandLine.cpp
            CommandLine.h
            CommonInclude.h
            DEM2JSON.cpp
            DEMMatrix.cpp
            DEMMatrix.h
            GeoTiffReader.cpp
            GeoTiffReader.h
            Matrix.h
        .cproject
        .project

Eclipse CDT�Ńv���W�F�N�g�t�@�C�����J���ăr���h���܂��B

## �R���o�[�g���@

�쐬���ꂽ�I�u�W�F�N�g�̃R�}���h�p�����[�^�͉��L�̒ʂ�ł��B
`DEM2JSON -D path/to/ASTER -J path/to/JSON [-S]`

    *options
        -D : ASTER �f�[�^�̃p�X�BASTER��zip�t�@�C���̓������p�X�ł��B���̃p�X�̃t�@�C����S�ĕϊ����܂��B
        -J : �R���o�[�g����JSON�f�[�^�̏o�̓p�X�ł��B
        -S : �쐬����W���f�[�^���K�E�V�A���t�B���^�ŃX���[�V���O���邩���w�肵�܂��B

## �W���f�[�^�̗��p���@

js�t�@�C���������ł��B

## LICENSE

���̃X�N���v�g��MIT���C�Z���X�Ƃ��܂��B
LICENSE �t�@�C���Q�ƁB

## �A����Ȃ�

[email :] mmgithub@gmail.com
[web :] http://world-route.sakura.ne.jp/
