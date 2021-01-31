from django.db import models
from django.urls import reverse

# Create your models here.
class Drink(models.Model):
    drink_name = models.CharField(max_length=30, blank=True, null=True)
    fl_oz = models.FloatField(blank=True, null=True)
    g_sugar = models.FloatField(blank=True, null=True)
    g_floz = models.FloatField(blank=True, null=True)
    mg_caffeine = models.FloatField(blank=True, null=True)
    mg_floz = models.FloatField(blank=True, null=True)
    id = models.IntegerField(primary_key=True)

    class Meta:
        managed = False
        db_table = 'Drink'
    def __str__(self):
        return self.drink_name
    def get_absolute_url(self):
        return reverse('drink_edit', kwargs={'pk':self.pk})

class Caffeine(models.Model):
    drink_name = models.CharField(max_length=30, blank=True, null=True)
    fl_oz = models.FloatField(blank=True, null=True)
    mg_caffeine = models.FloatField(blank=True, null=True)
    mg_floz = models.FloatField(blank=True, null=True)
    id = models.IntegerField(primary_key=True)

    class Meta:
        managed = False
        db_table = 'caffeine'
    def __str__(self):
        return self.drink_name
    def get_absolute_url(self):
        return reverse('drink_edit', kwargs={'pk':self.pk})

class Sugar(models.Model):
    drink_name = models.CharField(max_length=30, blank=True, null=True)
    fl_oz = models.FloatField(blank=True, null=True)
    g_sugar = models.FloatField(blank=True, null=True)
    g_floz = models.FloatField(blank=True, null=True)
    id = models.IntegerField(primary_key=True)

    class Meta:
        managed = False
        db_table = 'sugar'
    def __str__(self):
        return self.drink_name
    def get_absolute_url(self):
        return reverse('drink_edit', kwargs={'pk':self.pk})
    
class User(models.Model):
    user_id = models.CharField(primary_key=True, blank=False, max_length=10)
    password = models.CharField(max_length=16, blank=False)
    name = models.CharField(max_length=40, blank=True, null=True)
    drink_name = models.CharField(max_length=30, blank=True, null=True)
    gender = models.CharField(max_length=1, blank=True, null=True)
    height = models.FloatField(blank=True, null=True)
    weight = models.FloatField(blank=True, null=True)
    age = models.IntegerField(blank=True, null=True)
    is_pregnant = models.IntegerField(blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'user'
    def __str__(self):
        return self.user_id
    def get_absolute_url(self):
        return reverse('user_info', kwargs={'pk':self.pk})