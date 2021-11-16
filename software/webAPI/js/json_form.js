/**
 * form��json�ɕϊ�����
 */
  function jsonSend(){
    console.log('jsonSend');
    // �@json�`���擾
    var text = JSON.stringify($(this).jsForm("get"), undefined, 2);
  
    // �A���M
    $.ajax({
      'url':           this.action,
      'type':          'post',
      'dataType':      'json',
      'contentType':   'application/json',
      'data':          text
    })  
  
    // �A-a ���� - ���ʏo��
    .done(function(data, textStatus, xhr){
      $('#display').text(JSON.stringify(data, undefined, 2));
    })
  
    // �A-b ���s - �G���[�o��
    .fail(function(xhr, textStatus, errorThrown){
      var text = JSON.stringify(
        {
          "XMLHttpRequest": xhr,
          "textStatus": textStatus,
          "errorThrown": errorThrown
        }, 
        undefined, 2
      );
      $('#display').text(text);
    });
  
    // �B �L�����Z��
    // �ʏ�t�H�[��(application/x-www-form-urlencoded)�̑��M��}�~
    return false;
  
  };
  
  /**
   * �t�H�[���̏�����
   */
  function formInit(){
    var data = {
        "state": "state",
        "ymd": "ymd",
        "time": "time",
    };
    $('form').jsForm({"data": data});
    $('form').submit(jsonSend);
  };
  
  /*
   * �S�̏�����
   */
  jQuery(function($){

    formInit();
    
    });