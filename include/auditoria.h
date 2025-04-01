#ifndef AUDITORIA_H
#define AUDITORIA_H

void solicitarYRegistrarAuditoria();
void solicitarYListarAuditoria();
void registrarRegistroAuditoria(int idUsuario, const char *accionCodigo, const char *accionDescripcion,
    const char *tipoObjeto, int idObjeto, const char *detalles,
    const char *severidad, const char *estado);
#endif
