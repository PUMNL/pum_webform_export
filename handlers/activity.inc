<?php

class PumWebformExportActivityHandler {
  
  protected $activity_type_id;
  
  protected $activity_status_id;
  
  public function __construct() {
    $this->activity_type_id = civicrm_api3('OptionGroup', 'getvalue', array('name' => 'activity_type', 'return' => 'id'));
    $this->activity_status_id = civicrm_api3('OptionGroup', 'getvalue', array('name' => 'activity_status', 'return' => 'id'));
  }
  
  public static function export(&$node, $original_node) {
    $e = new PumWebformExportActivityHandler();
    if (isset($node->webform_civicrm['data']['activity']) && is_array($node->webform_civicrm['data']['activity'])) {
      foreach($node->webform_civicrm['data']['activity'] as $i => $activity) {
        $node->webform_civicrm['data']['activity'][$i] = $e->exportActivity($activity);
      }
    }
  }
  
  public static function import(&$node, $original_node) {
    $e = new PumWebformExportActivityHandler();
    if (isset($node->webform_civicrm['data']['activity']) && is_array($node->webform_civicrm['data']['activity'])) {
      foreach($node->webform_civicrm['data']['activity'] as $i => $activity) {
        $node->webform_civicrm['data']['activity'][$i] = $e->importActivity($activity);
      }
    }
  }
  
  public function exportActivity($activity) {
    foreach($activity['activity'] as $i => $c) {
      $type = $this->getActivityTypeName($activity['activity'][$i]['activity_type_id']);
      if ($type) {
        $activity['activity'][$i]['activity_type_id'] = $type;
      }
      $status = $this->getActivityStatusName($activity['activity'][$i]['status_id']);
      if ($status) {
        $activity['activity'][$i]['status_id'] = $status;
      }
      
    }
    if (isset($activity['existing_activity_status']) && is_array($activity['existing_activity_status'])) {
      $existing_status = array();
      foreach($activity['existing_activity_status'] as $status_id) {
        $k = $this->getActivityStatusName($status_id);
        $existing_status[$k] = $k;
      }
      $activity['existing_activity_status'] = $existing_status;
    }
    return $activity;
  }
  
  public function importActivity($activity) {
    foreach($activity['activity'] as $i => $c) {
      $type = $this->getActivityTypeId($activity['activity'][$i]['activity_type_id']);
      if ($type) {
        $activity['activity'][$i]['activity_type_id'] = $type;
      }
      $status = $this->getActivityStatusId($activity['activity'][$i]['status_id']);
      if ($status) {
        $activity['activity'][$i]['status_id'] = $status;
      }
      
    }
    if (isset($activity['existing_activity_status']) && is_array($activity['existing_activity_status'])) {
      $existing_status = array();
      foreach($activity['existing_activity_status'] as $status_id) {
        $k = $this->getActivityStatusId($status_id);
        $existing_status[$k] = $k;
      }
      $activity['existing_activity_status'] = $existing_status;
    }
    return $activity;
  }
  
  protected function getActivityTypeName($id) {
    try {
      return civicrm_api3('OptionValue', 'getvalue', array('value' => $id, 'return' => 'name', 'option_group_id' => $this->activity_type_id));
    } catch (Exception $ex) {
      //do nothing
    }
    return false;
  }
  
  protected function getActivityStatusName($id) {
    try {
      return civicrm_api3('OptionValue', 'getvalue', array('value' => $id, 'return' => 'name', 'option_group_id' => $this->activity_status_id));
    } catch (Exception $ex) {
      //do nothing
    }
    return false;
  }
  
  protected function getActivityTypeId($name) {
    try {
      return civicrm_api3('OptionValue', 'getvalue', array('name' => $name, 'return' => 'value', 'option_group_id' => $this->activity_type_id));
    } catch (Exception $ex) {
      //do nothing
    }
    return false;
  }
  
  protected function getActivityStatusId($name) {
    try {
      return civicrm_api3('OptionValue', 'getvalue', array('name' => $name, 'return' => 'value', 'option_group_id' => $this->activity_status_id));
    } catch (Exception $ex) {
      //do nothing
    }
    return false;
  }

  
}