from django.db import connection

# Figure out how to add weight etc...
def add_user(user_id, password, name, drink_name, gender, height, weight, age, is_pregnant):
    with connection.cursor() as cursor:
        cursor.execute("INSERT INTO user VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s)", [user_id, password, name, drink_name, gender, height, weight, age, is_pregnant])
    return

def get_drinks():
    with connection.cursor() as cursor:
        cursor.execute("SELECT caffeine.drink_name, caffeine.fl_oz, g_sugar, g_floz, mg_caffeine, mg_floz, caffeine.id FROM caffeine JOIN sugar on sugar.id = caffeine.id")
        query = cursor.fetchall()
    return query

def calculate_recs(user_id):
    with connection.cursor() as cursor:
        cursor.execute("SELECT * FROM user WHERE user_id = %s",[user_id])
        query = cursor.fetchone()
    drink_name = query[3]
    with connection.cursor() as cursor:
        cursor.execute("SELECT * FROM caffeine JOIN sugar on sugar.id = caffeine.id WHERE caffeine.drink_name = %s", [drink_name])
        drink_info = cursor.fetchone()
    curr_caff = drink_info[2]
    curr_sug = drink_info[7]
    gender = query[4]
    height = query[5]
    weight = query[6]
    age = query[7]
    pregnant = query[8]
    height_cm = height * 2.54
    weight_kg = weight * .45392
    bmi = (703 * weight) / (height**2)
    daily_sug_gram_rec = 0
    daily_sug_gram_max = 0
    #10 % of daily calories
    if bmi < 27:
        if gender == 'M':
            daily_calories = 10*weight_kg +6.25*height_cm - 5*age +5
        else:
            daily_calories = 10*weight_kg +6.25*height_cm - 5*age - 161
        daily_sug_cal = .1*daily_calories
        daily_sug_gram_max = .25*daily_sug_cal
        daily_sug_gram_rec = .12959782*daily_sug_cal
    if pregnant:
        daily_caf_max = 200
        daily_caf_rec = 45
    if age < 12:
        daily_caf_max = 45
        daily_caf_rec = 30
    elif age < 19:
        daily_caf_max = 100
        daily_caf_rec = 80
    else:
        daily_caf_max = 400
        curve = develop_curve()
        daily_caf_rec = curve[age-19]

    return daily_sug_gram_rec, daily_sug_gram_max, daily_caf_rec, daily_caf_max, curr_caff, curr_sug

def develop_curve():
    diff_min_prime = 30-19
    increment_level = 80/diff_min_prime
    vals = [120]
    for i in range(1, diff_min_prime+1):
        vals.append(vals[i-1]+increment_level)
    decrement_level = 200/(68)
    for i in range(12, 79):
        vals.append(vals[i-1]-decrement_level)
    return vals


        
        