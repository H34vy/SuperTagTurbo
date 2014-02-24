//=============================================================================
//	Constants.h
//	Chris Prosser
//		Many macros inspired by Dr. Birmingham
//=============================================================================
#ifndef CONSTANTS_H
#define CONSTANTS_H

//Defines
#define Color		D3DXCOLOR
#define Vector3		D3DXVECTOR3
#define Matrix		D3DXMATRIX

#define Identity	D3DXMatrixIdentity
#define Translate	D3DXMatrixTranslation
#define RotateX		D3DXMatrixRotationX
#define RotateY		D3DXMatrixRotationY
#define RotateZ		D3DXMatrixRotationZ
#define Scale		D3DXMatrixScaling
#define LookAt		D3DXMatrixLookAtLH

#define ToRadian	D3DXToRadian
#define ToDegree	D3DXToDegree

#define Device		ID3D10Device
#define Buffer		ID3D10Buffer
#define Technique	ID3D10EffectTechnique
#define MatrixVar	ID3D10EffectMatrixVariable

#define ZERO		Vector3(0,0,0);
#define PI			3.14159;

#endif