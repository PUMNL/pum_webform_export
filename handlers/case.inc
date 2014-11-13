<?php

class PumWebformExportCaseHandler {
  
  protected $case_type_id;
  
  protected $case_status_id;
  
  public function __construct() {
    $this->case_type_id = civicrm_api3('OptionGroup', 'getvalue', array('name' => 'case_type', 'return' => 'id'));
    $this->case_status_id = civicrm_api3('OptionGroup', 'getvalue', array('name' => 'case_status', 'return' => 'id'));
  }
  
  public static function export(&$node, $original_node) {
    $e = new PumWebformExportCaseHandler();
    if (isset($node->webform_civicrm['data']['case']) && is_array($node->webform_civicrm['data']['case'])) {
      foreach($node->webform_civicrm['data']['case'] as $i => $case) {
        $node->webform_civicrm['data']['case'][$i] = $e->exportCase($case);
      }
    }
  }
  
  public static function import(&$node, $original_node) {
    $e = new PumWebformExportCaseHandler();
    if (isset($node->webform_civicrm['data']['case']) && is_array($node->webform_civicrm['data']['case'])) {
      foreach($node->webform_civicrm['data']['case'] as $i => $case) {
        $node->webform_civicrm['data']['case'][$i] = $e->importCase($case);
      }
    }
  }
  
  public function exportCase($case) {
    foreach($case['case'] as $i => $c) {
      $type = $this->getCaseTypeName($case['case'][$i]['case_type_id']);
      if ($type) {
        $case['case'][$i]['case_type_id'] = $type;
      }
      $status = $this->getCaseStatusName($case['case'][$i]['status_id']);
      if ($status) {
        $case['case'][$i]['status_id'] = $status;
      }
      
    }
    if (isset($case['existing_case_status']) && is_array($case['existing_case_status'])) {
      $existing_status = array();
      foreach($case['existing_case_status'] as $status_id) {
        $k = $this->getCaseStatusName($status_id);
        $existing_status[$k] = $k;
      }
      $case['existing_case_status'] = $existing_status;
    }
    return $case;
  }
  
  public function importCase($case) {
    foreach($case['case'] as $i => $c) {
      $type = $this->getCaseTypeId($case['case'][$i]['case_type_id']);
      if ($type) {
        $case['case'][$i]['case_type_id'] = $type;
      }
      $status = $this->getCaseStatusId($case['case'][$i]['status_id']);
      if ($status) {
        $case['case'][$i]['status_id'] = $status;
      }
      
    }
    if (isset($case['existing_case_status']) && is_array($case['existing_case_status'])) {
      $existing_status = array();
      foreach($case['existing_case_status'] as $status_id) {
        $k = $this->getCaseStatusId($status_id);
        $existing_status[$k] = $k;
      }
      $case['existing_case_status'] = $existing_status;
    }
    return $case;
  }
  
  protected function getCaseTypeName($id) {
    try {
      return civicrm_api3('OptionValue', 'getvalue', array('value' => $id, 'return' => 'name', 'option_group_id' => $this->case_type_id));
    } catch (Exception $ex) {
      //do nothing
    }
    return false;
  }
  
  protected function getCaseStatusName($id) {
    try {
      return civicrm_api3('OptionValue', 'getvalue', array('value' => $id, 'return' => 'name', 'option_group_id' => $this->case_status_id));
    } catch (Exception $ex) {
      //do nothing
    }
    return false;
  }
  
  protected function getCaseTypeId($name) {
    try {
      return civicrm_api3('OptionValue', 'getvalue', array('name' => $name, 'return' => 'value', 'option_group_id' => $this->case_type_id));
    } catch (Exception $ex) {
      //do nothing
    }
    return false;
  }
  
  protected function getCaseStatusId($name) {
    try {
      return civicrm_api3('OptionValue', 'getvalue', array('name' => $name, 'return' => 'value', 'option_group_id' => $this->case_status_id));
    } catch (Exception $ex) {
      //do nothing
    }
    return false;
  }

  
}