@echo off
setlocal enabledelayedexpansion

REM === ��� ���� ===
set ROOT_DIR=%~dp0
set PROTO_GEN_DIR=%ROOT_DIR%ProtoGenerator
set PYTHON_SCRIPTS_DIR=%ROOT_DIR%PythonScripts
set CPP_DIR=%PYTHON_SCRIPTS_DIR%\CPP
set CSHARP_DIR=%PYTHON_SCRIPTS_DIR%\CSharp
set GENERATED_DIR=%ROOT_DIR%GeneratedFiles

set PROTOC=%PROTO_GEN_DIR%\protoc.exe
set PROTO_FILE=%PROTO_GEN_DIR%\Protocol.proto

set DEST_CPP=%GENERATED_DIR%\CPP
set DEST_CS=%GENERATED_DIR%\CSharp

REM === [1] proto_definitions.xlsx ���� ===
echo [1] ���� ��: proto_definitions.xlsx -> ProtoGenerator
copy /Y "%ROOT_DIR%proto_definitions.xlsx" "%PROTO_GEN_DIR%"

REM === [2] generate_proto.exe ���� ===
echo [2] ���� ��: generate_proto.exe
pushd "%PROTO_GEN_DIR%"
"%PROTO_GEN_DIR%\generate_proto.exe"
popd

REM === [3] proto_definitions.xlsx ���� ===
echo [3] ���� ��: proto_definitions.xlsx
del /F /Q "%PROTO_GEN_DIR%\proto_definitions.xlsx"

REM === [4] protoc.exe ���� �� C++ / C# �ڵ� ���� ===

REM ��� ���� ����: CPP
if exist "%DEST_CPP%" (
    if not exist "%DEST_CPP%\nul" (
        echo �߸� ������ GeneratedFiles\CPP ���� ���� ��...
        del /F /Q "%DEST_CPP%"
    )
)
if not exist "%DEST_CPP%\" (
    mkdir "%DEST_CPP%"
)

REM ��� ���� ����: CSharp
if exist "%DEST_CS%" (
    if not exist "%DEST_CS%\nul" (
        echo �߸� ������ GeneratedFiles\CSharp ���� ���� ��...
        del /F /Q "%DEST_CS%"
    )
)
if not exist "%DEST_CS%\" (
    mkdir "%DEST_CS%"
)

echo [4] protoc.exe�� ����Ͽ� C++ �ڵ� ����
"%PROTOC%" --proto_path="%PROTO_GEN_DIR%" --cpp_out="%DEST_CPP%" "%PROTO_FILE%"
if errorlevel 1 (
    echo [����] C++ �ڵ� ���� ����
    pause
    exit /b
)

echo [5] protoc.exe�� ����Ͽ� C# �ڵ� ����
"%PROTOC%" --proto_path="%PROTO_GEN_DIR%" --csharp_out="%DEST_CS%" "%PROTO_FILE%"
if errorlevel 1 (
    echo [����] C# �ڵ� ���� ����
    pause
    exit /b
)

REM === [6] Protocol.proto�� CPP / CSharp ���丮�� ���� (Python ��ũ��Ʈ��) ===
echo [6] ���� ��: Protocol.proto -> CPP, CSharp
copy /Y "%PROTO_FILE%" "%CPP_DIR%"
copy /Y "%PROTO_FILE%" "%CSHARP_DIR%"

REM === [7] CPP Python ��ũ��Ʈ ���� ===
echo [7] ���� ��: CPP Python Scripts
pushd "%CPP_DIR%"
python generate_packet_proc.py
python generate_packets.py
python generate_proto_struct.py
popd

REM === [8] CSharp Python ��ũ��Ʈ ���� ===
echo [8] ���� ��: CSharp Python Scripts
pushd "%CSHARP_DIR%"
python generate_packet_handler.py
python generate_packet_manager.py
popd

REM === [9] CPP Python ��� ���� �̵� ===
echo [9] CPP Python ��� �̵� -> GeneratedFiles\CPP
move /Y "%CPP_DIR%\*.cpp" "%DEST_CPP%\"
move /Y "%CPP_DIR%\*.h" "%DEST_CPP%\"
del /F /Q "%CPP_DIR%\*.proto" >nul 2>&1

REM === [10] CSharp Python ��� ���� �̵� ===
echo [10] CSharp Python ��� �̵� -> GeneratedFiles\CSharp
move /Y "%CSHARP_DIR%\*.cs" "%DEST_CS%\"
del /F /Q "%CSHARP_DIR%\*.proto" >nul 2>&1

REM === [11] ��Ʈ�� �߸� ������ CPP ���� ���� ===
if exist "%ROOT_DIR%CPP" (
    if not exist "%ROOT_DIR%CPP\" (
        del /F /Q "%ROOT_DIR%CPP"
    )
)

echo [12] ��� �۾� �Ϸ�
pause
