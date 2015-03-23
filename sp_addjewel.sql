DELIMITER $$

DROP PROCEDURE IF EXISTS `db_lobbylog`.`sp_addjewel` $$
CREATE PROCEDURE `db_lobbylog`.`sp_addjewel` (p_pid int,p_gameid int,p_type int,p_num int,p_time int)
BEGIN
  insert into tb_playerlog (pl_pid,pl_gameid,pl_gametype,pl_amount,pl_timestamp,pl_note,pl_variable) values(p_pid,p_gameid,p_gametype,p_time,p_num);
END $$

DELIMITER ;